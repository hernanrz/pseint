#ifndef STRFUNCS_HPP
#define STRFUNCS_HPP
#include <string>
#include <vector>

//std::string FirstWord(const std::string &s);
bool LastCharIs(const std::string &s, char c);
void EraseLastChar(std::string &s);

void LeftTrim(std::string &s);
void RightTrim(std::string &s);
void Trim(std::string &s);

std::string ToUpper(std::string a);
std::string ToLower(std::string a);

inline char ToUpper(const char c) {
	if (c>96 && c<123) return c-32;
	if (c=='\xE1') return '\xC1'; // á -> Á
	if (c=='\xE9') return '\xC9'; // é -> É
	if (c=='\xED') return '\xCD'; // í -> Í
	if (c=='\xF3') return '\xD3'; // ó -> Ó
	if (c=='\xFA') return '\xDA'; // ú -> Ú
	if (c=='\xF1') return '\xD1'; // ñ -> Ñ
	if (c=='\xFC') return '\xDC'; // ü -> Ü
	return c;
}

// determina si un caracter (que debe venir en may�sculas es letra (incluye acentos y �s)
inline bool EsLetra(const char &_c, bool incluir_numeros=false, bool allow_accents = true) {
	return (_c>='A' && _c<='Z') or (allow_accents and (_c=='\xC1'||_c=='\xC9'||_c=='\xCD'||_c=='\xD3'||_c=='\xDA'||_c=='\xD1'||_c=='\xDC') ) || _c=='_'  || (incluir_numeros && _c>='0' && _c<='9');
}

bool LeftCompare(std::string a, std::string b); /// @todo: eliminar o mover a keywords.cpp
bool RightCompare(std::string a, std::string b); /// @todo: eliminar o mover a keywords.cpp

int matchParentesis(const std::string &src, int p);
std::vector<std::string> splitArgsList(const std::string &args);


std::string MkErrorMsg(std::string msg, const std::string &arg, bool add_parentesis=false);
std::string MkErrorMsg(std::string msg, const std::string &arg1, const std::string &arg2);

// pasa todo a mayuscula, y elimina acentos y �s
char Normalize(char c);

// pasa todo a mayusculas, preservando acentos y ns
char NormalizeKA(char c);

#endif

