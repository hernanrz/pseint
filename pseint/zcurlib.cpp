#include "global.h"
#include <iostream>
#include <cstdlib>
#ifdef __WIN32__
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

void lnx_setBackColor(int color);	
void lnx_setForeColor(int color);
void lnx_clrscr();
void lnx_gotoXY(int x, int y);
int lnx_getKey(void);
void lnx_hideCursor();
void lnx_showCursor();
void lnx_setTitle(const char* title);
void win_setBackColor(int color);	
void win_setForeColor(int color);
void win_clrscr();
void win_gotoXY(int x, int y);
int win_getKey(void);
void win_hideCursor();
void win_showCursor();
void win_setTitle(const char* title);

void setBackColor(int color) {
#ifdef __WIN32__
	if (!for_pseint_terminal)
		win_setBackColor(color);
	else
#endif
		lnx_setBackColor(color);
}

void setForeColor(int color) {
#ifdef __WIN32__
	if (!for_pseint_terminal)
		win_setForeColor(color);
	else
#endif
		lnx_setForeColor(color);
}

void clrscr() {
#ifdef __WIN32__
	if (!for_pseint_terminal)
		win_clrscr();
	else
#endif
		lnx_clrscr();
}

void gotoXY(int x, int y) {
#ifdef __WIN32__
	if (!for_pseint_terminal)
		win_gotoXY(x,y);
	else
#endif
		lnx_gotoXY(x,y);	
}

void hideCursor() {
#ifdef __WIN32__
	if (!for_pseint_terminal)
		win_hideCursor();
	else
#endif
		lnx_showCursor();	
}
void showCursor() {
#ifdef __WIN32__
	if (!for_pseint_terminal)
		win_showCursor();
	else
#endif
		lnx_showCursor();
}

void setTitle(const char* title) {
#ifdef __WIN32__
	if (!for_pseint_terminal)
		win_setTitle(title);
	else
#endif
		lnx_setTitle(title);	
}

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	EM_ASYNC_JS(void, js_waitMs, (int ms), {
		if (typeof Module !== 'undefined' && typeof Module.onWait === 'function') {
			return Promise.resolve(Module.onWait(ms));
		}

		return new Promise((resolve) => {
			setTimeout(resolve, Math.max(0, ms));
		});
	});

	EM_ASYNC_JS(int, js_getKey, (), {
		if (typeof Module !== 'undefined' && typeof Module.onGetKey === 'function') {
			return Promise.resolve(Module.onGetKey()).then((value) => (
				typeof value === 'number' ? value : 10
			));
		}

		return new Promise((resolve) => {
			window.alert('Presione una tecla para continuar');
			resolve(10);
		});
	});

	EM_ASYNC_JS(char*, js_getLine, (), {
		const toUtf8Ptr = (text) => {
			const safeText = typeof text === 'string' ? text : "";
			const len = lengthBytesUTF8(safeText) + 1;
			const ptr = _malloc(len);
			stringToUTF8(safeText, ptr, len);
			return ptr;
		};

		if (typeof Module !== 'undefined' && typeof Module.onGetLine === 'function') {
			return Promise.resolve(Module.onGetLine()).then((value) => toUtf8Ptr(value));
		}

		return new Promise(resolve => {
			const input = document.createElement('input');
			input.type = 'text';
			input.style.position = 'fixed';
			input.style.bottom = '10px';
			input.style.left = '10px';
			document.body.appendChild(input);
			input.focus();
			input.addEventListener('keydown', (event) => {
				if (event.key === 'Enter') {
					resolve(toUtf8Ptr(input.value));
					document.body.removeChild(input);
				}
			});
		});
	});
#endif 

void waitMs(int ms) {
	if (ms <= 0) return;

	#ifdef __EMSCRIPTEN__
		js_waitMs(ms);
	#else
		#ifdef __WIN32__
			Sleep(ms);
		#else
			usleep(ms * 1000);
		#endif
	#endif
}

int getKey(void) {
	#ifdef __EMSCRIPTEN__
		if (for_pseint_terminal) {
			cout<<"\033[zk";
			return js_getKey();
		}
	#endif

	if (for_pseint_terminal) {
		cout<<"\033[zk";
		return cin.get();
	} else {
#ifdef __WIN32__
		return win_getKey();
#else
		return lnx_getKey();
#endif
	}
}

string getLine() {
	# ifndef __EMSCRIPTEN__
		if (for_pseint_terminal) cout<<"\033[zl";
		string s; getline(cin,s); return s;
	# else
		char* line = js_getLine();
		string s = line ? string(line) : string();
		free(line);
		return s;
	# endif
}
