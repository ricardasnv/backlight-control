#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"

void usage(const char*);
void fail(const char*, int);
void applog(const char*);

void create_file(const char*, float);
bool read_brightness(const char*, float*);
void write_brightness(const char*, float, bool);
bool is_number(const char*);

int main(int argc, char** argv) {
	if (argc < 3) {
		usage(argv[0]);
	}

	if (is_number(argv[2]) == false) {
		char* failstr = (char*)malloc(256 * sizeof(char));
		snprintf(failstr, 256, "main(): not a number - argv[2]=\"%s\"", argv[2]);
		fail(failstr, 1);
	}

	float value = atof(argv[2]); // Given value
	float brightness;
	
	// Home path ($HOME)
	char* home_path = (char*)malloc(256 * sizeof(char));
	strcpy(home_path, getenv("HOME"));

	// Brightness file directory
	char* br_dir = (char*)malloc(256 * sizeof(char));
	strcpy(br_dir, home_path);
	strcat(br_dir, BRDIR);

	// Path to default brightness file
	char* default_br_path = (char*)malloc(256 * sizeof(char));
	strcpy(default_br_path, home_path);
	strcat(default_br_path, DEFBRFILE);

	// Path to current brightness file
	char* current_br_path = (char*)malloc(256 * sizeof(char));
	strcpy(current_br_path, home_path);
	strcat(current_br_path, CURBRFILE);

	// Try to get current brightness
	if (!read_brightness(current_br_path, &brightness)) {
		// Try to get default brightness
		if (!read_brightness(default_br_path, &brightness)) {
			// Create default_br_path with value "100"
			brightness = 100;
			create_file(default_br_path, brightness);

			char* logstr = (char*)malloc(256 * sizeof(char));
			snprintf(logstr, 256, "main(): created new default brightness file at %s", default_br_path);
			applog(logstr);
		}

		create_file(current_br_path, brightness);

		char* logstr = (char*)malloc(256 * sizeof(char));
		snprintf(logstr, 256, "main(): created new current brightness file at %s", current_br_path);
		applog(logstr);
	}

	// Interpret input
	if (strcmp(argv[1], "add") == 0) {
		brightness += value;
	} else if (strcmp(argv[1], "sub") == 0) {
		brightness -= value;
	} else if (strcmp(argv[1], "mul") == 0) {
		brightness *= value;
	} else if (strcmp(argv[1], "div") == 0) {
		brightness /= value;
	} else if (strcmp(argv[1], "set") == 0) {
		brightness = value;
	} else {
		char* failstr = (char*)malloc(256 * sizeof(char));
		snprintf(failstr, 256, "main(): bad argument argv[1]=\"%s\"", argv[1]);
		fail(failstr, 1);
	}

	write_brightness(current_br_path, brightness, true);
	write_brightness(SYSFILE, brightness, false);

	return 0;
}

// Show usage message and exit
void usage(const char* argv0) {
	printf("Usage: %s [add|sub|mul|div|set] [VALUE]\n", argv0);
	fail("usage()", 1);
}

// Exit with given return value
void fail(const char* reason, int ret) {
	applog(reason);
	applog("Program dead :^(");
	exit(ret);
}

// TODO: append given string to log file
void applog(const char* str) {
	printf("%s\n", str);
}

// Create file 'filename' containing 'value'
void create_file(const char* filename, float value) {
	FILE *f = fopen(filename, "r");
	fprintf(f, "%f", value);
	fclose(f);
}

// Read a single float value into 'brightness' from file 'filename'
bool read_brightness(const char* filename, float* brightness) {
	FILE *f = fopen(filename, "r");

	if (f == NULL) {
		char* logstr = (char*)malloc(256 * sizeof(char));
		snprintf(logstr, 256, "read_brightness(): failed to open file %s for reading", filename);
		applog(logstr);

		return false;
	}

	fscanf(f, "%f", brightness);
	fclose(f);

	return true;
}

// Write a single floating point value 'brightness' into 'filename'
// If write_as_float is unset, omit everything after the point
void write_brightness(const char* filename, float brightness, bool write_as_float) {
	FILE *f = fopen(filename, "w");
	float max_brightness;

	if (!read_brightness(SYSMAXFILE, &max_brightness)) {
		char* failstr = (char*)malloc(256 * sizeof(char));
		snprintf(failstr, 256, "write_brightness(): failed to open file %s for reading", SYSMAXFILE);
		fail(failstr, 1);
	}

	if (f == NULL) {
		char* failstr = (char*)malloc(256 * sizeof(char));
		snprintf(failstr, 256, "write_brightness(): Failed to open file %s for writing. Insufficient permissions?", filename);
		fail(failstr, 1);
	}

	char format_string[6] = "%.0f\n";

	if (write_as_float == true) {
		strcpy(format_string, "%f\n");
	}

	if (brightness <= max_brightness && brightness >= MIN_BRIGHTNESS) {
		fprintf(f, format_string, brightness);
	} else if (brightness > max_brightness) {
		fprintf(f, format_string, max_brightness);
	} else if (brightness < MIN_BRIGHTNESS) {
		fprintf(f, format_string, MIN_BRIGHTNESS);
	}

	fclose(f);
}

// Returns true if given string only contains bytes 48-57 or 46
// (digits or '.')
bool is_number(const char* str) {
	bool result = true;

	for (int i = 0; i < strlen(str); i++) {
		if ((str[i] < 48 || str[i] > 57) && str[i] != 46) {
			result = false;
		}
	}

	return result;
}

