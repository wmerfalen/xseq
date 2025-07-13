#include <iostream>
#include <string>
#include <vector>

#define BINARY "xseq"
enum range_preference {
	RT_NUMERIC,
	RT_ALPHA,
};
struct program_options {
	range_preference range_type;
	int first;
	int last;
	int increment;
	bool blasphemy; // if true, first is 1, otherwise first is zero
	std::string separator;
	bool expecting_different_separator;
};

#if defined(DEBUG_PROGRAM_OPTIONS)
void dump_opts(const program_options& opts) {
	printf("[range_type:%s]\n",opts.range_type == RT_ALPHA ? "alpha" : "numeric");
	if(opts.range_type == RT_ALPHA) {
		printf("[first:%c]\n",(char)opts.first);
		printf("[last:%c]\n",(char)opts.last);
		printf("[increment:%d]\n",opts.increment);
	} else {
		printf("[first:%d]\n",opts.first);
		printf("[last:%d]\n",opts.last);
		printf("[increment:%d]\n",opts.increment);
	}
}
void dump_vector(const std::vector<std::string>& non_flag_parameters) {
	printf("[dump_vector(size:%lu)]\n",non_flag_parameters.size());
	for(std::size_t i=0; i < non_flag_parameters.size(); i++) {
		printf("[%lu]=>'%s'\n",i,non_flag_parameters[i].c_str());
	}
	printf("[end of dump_vector]\n");
}
#else
#define dump_opts(A)
#define dump_vector(non_flag_parameters)
#endif

int print_range(program_options& options,const std::vector<std::string>& params) {
	if(options.increment == 0) {
		printf("%s: invalid Zero increment value: ‘0’\n",BINARY);
		printf("Try '%s --help' for more information.\n",BINARY);
		return 1;
	}
	if(options.range_type == RT_NUMERIC) {
		if(options.last < options.first) {
			return 0;
		}
		for(int i=options.first; i <= options.last; i += options.increment) {
			printf("%d%s",i,options.separator.c_str());
		}
		dump_opts(options);
		return 0;
	}
	if(options.range_type == RT_ALPHA) {
		for(int i=options.first; i <= options.last; i += options.increment) {
			printf("%c%s",(char)i,options.separator.c_str());
		}
		dump_opts(options);
		return 0;
	}
	return 0;
}
void usage() {
	const char* bin = BINARY;
	std::cerr
	        <<     "Usage: " << bin << " [OPTION]... <LAST>\n"
	            << " or:   " << bin << " [OPTION]... <FIRST> <LAST>\n"
	            << " or:   " << bin << " [OPTION]... <FIRST> <INCREMENT> <LAST>\n"
	            << "Print numbers or letters from FIRST to LAST, in steps of INCREMENT.\n"
	            << "\n"
	            << "  -a, --alpha       Treat FIRST and LAST as letters.\n"
	            << "  -z, --zero        When LAST is the only argument, assume FIRST is zero\n"
	            << "  -s, --separator=STRING   use STRING to separate numbers (default: \\n)\n"
	            << "  -h, --help        display this help and exit\n"
	            << "  -v, --version     output version information and exit\n"
	            << "\n"
	            << " <INCREMENT> Works for both numbers and letters (when -a)\n"
	            << "\n"
	            << "  If FIRST and LAST are letters, the range will be interpretted as if -a (or --alpha) was passed as an argument\n"
	            << "  As a result, the following examples are all equivalent:\n"
	            << "   " << bin << " -a A Z\n"
	            << "   " << bin << " A Z\n"
	            << " When FIRST and LAST are letters, INCREMENT can be used without explicitly specifying -a or --alpha.\n"
	            << " The following invocation will skip print every 3rd letter between A and Z:\n"
	            << "   " << bin << " A 3 Z\n"
	            << "\n";
}

int main(int argc,char** argv) {
	program_options options;
	options.range_type = RT_NUMERIC;
	options.first = 1;
	options.last= 1;
	options.increment = 1;
	options.blasphemy = true; // To be compatible with legacy coreutils seq
	options.expecting_different_separator = false;
	options.separator = "\n";
	const std::string long_separator_option = "--separator=";
	const std::size_t long_separator_option_length = long_separator_option.length();
	std::vector<std::string> args;
	std::vector<std::string> non_flag_parameters;
	for(int i=0; i < argc; i++) {
		args.push_back(argv[i]);
	}
	if(args.size() < 2) {
		usage();
		return 0;
	}
	bool capture_separator = false;

	for(int i=1; i < args.size(); i++) {
		if(args[i].compare("-a") == 0 || args[i].compare("--alpha") == 0) {
			options.range_type = RT_ALPHA;
			continue;
		}
		if(args[i].compare("-z") == 0 || args[i].compare("--zero") == 0) {
			options.first = 0;
			options.blasphemy = false;
			continue;
		}
		if(args[i].compare("-h") == 0 || args[i].compare("--help") == 0) {
			usage();
			return 0;
		}
		if(args[i].compare("-v") == 0 || args[i].compare("--version") == 0) {
			printf("%s 1.0\n",BINARY);
			printf("License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
			printf("This is free software: you are free to change and redistribute it.\n");
			printf("There is NO WARRANTY, to the extent permitted by law.\n");
			printf("\n");
			printf("This program is inspired by seq which is a coreutils program originally written by Ulrich Drepper.\n");
			printf("Written by William Merfalen\n");
			return 0;
		}
		if(args[i].compare("-s") == 0) {
			capture_separator = true;
			options.expecting_different_separator = true;
			continue;
		}
		if(capture_separator) {
			options.separator = args[i];
			capture_separator = false;
			options.expecting_different_separator = false;
			continue;
		}
		if(args[i].length() > long_separator_option_length && args[i].substr(0,long_separator_option_length).compare(long_separator_option.c_str()) == 0) {
			options.separator = args[i].substr(long_separator_option_length);
			options.expecting_different_separator = false;
			continue;
		}

		non_flag_parameters.push_back(args[i]);
	}
	dump_vector(non_flag_parameters);
	switch(non_flag_parameters.size()) {
		case 1:
			if(isalpha(non_flag_parameters[0][0]) && non_flag_parameters[0].length() == 1) {
				options.range_type = RT_ALPHA;
				options.first = '\0';
			}
			if(options.range_type == RT_NUMERIC) {
				options.last = atoi(non_flag_parameters[0].c_str());
			} else {
				options.last = non_flag_parameters[0][0];
			}
			break;
		case 2:
			if(isalpha(non_flag_parameters[0][0]) && isalpha(non_flag_parameters[1][0])) {
				options.range_type = RT_ALPHA;
			}
			if(options.range_type == RT_NUMERIC) {
				options.first = atoi(non_flag_parameters[0].c_str());
				options.last = atoi(non_flag_parameters[1].c_str());
			} else {
				options.first = non_flag_parameters[0][0];
				options.last = non_flag_parameters[1][0];
			}
			break;
		case 3:
			if(isalpha(non_flag_parameters[0][0]) && isalpha(non_flag_parameters[2][0])) {
				options.range_type = RT_ALPHA;
			}
			if(options.range_type == RT_NUMERIC) {
				options.first = atoi(non_flag_parameters[0].c_str());
				options.increment = atoi(non_flag_parameters[1].c_str());
				options.last = atoi(non_flag_parameters[2].c_str());
			} else {
				options.first = non_flag_parameters[0][0];
				options.increment = atoi(non_flag_parameters[1].c_str());
				options.last = non_flag_parameters[2][0];
			}
			break;
		default:
			break;
	}

	return print_range(options,non_flag_parameters);
}
