#include <string>
#include <vector>

#include "editor_runner.hpp"

int pseint_entrypoint(int argc, char* argv[]);

extern "C" int pseint_run_editor(const char *program_path, const char *profile_path, int syncheck_mode) {
	if (program_path == nullptr || program_path[0] == '\0') {
		return 1;
	}

	std::vector<std::string> arg_values;
	arg_values.reserve(5);
	arg_values.push_back("pseint");
	arg_values.push_back(program_path);
	arg_values.push_back("--nouser");

	if (profile_path != nullptr && profile_path[0] != '\0') {
		arg_values.emplace_back("--profile=");
		arg_values.back() += profile_path;
	}

	if (syncheck_mode != 0) {
		arg_values.push_back("--norun");
	}

	std::vector<char*> argv;
	argv.reserve(arg_values.size());
	for (auto &value : arg_values) {
		argv.push_back(const_cast<char*>(value.c_str()));
	}

	return pseint_entrypoint(static_cast<int>(argv.size()), argv.data());
}
