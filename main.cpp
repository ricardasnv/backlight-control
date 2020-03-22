#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

// Return codes
#define RET_NOROOT 1 // Program is not run as root
#define RET_NOARG 2  // Arguments are not given
#define RET_BADVAL 3 // argv[2] is not a number
#define RET_BADOP 4  // argv[1] does not specify a valid operation

const char* SYSFILE = "/sys/class/backlight/intel_backlight/brightness";
const char* SYSMAXFILE = "/sys/class/backlight/intel_backlight/max_brightness";
const char* CFGFILE = strcat(getenv("HOME"), "/.backlight");
const float MIN_BRIGHTNESS = 0.5;

bool read_brightness(const char*, float&);
void write_brightness(const char*, float, bool);
bool is_number(const char*);

int main(int argc, char** argv) {
	if (geteuid() != 0) {
		printf("You are not root, GTFO!\n");
		return RET_NOROOT;
	}

	if (argc < 3) {
		printf("Usage: %s [add|sub|mul|div|set] [VALUE]\n", argv[0]);
		return RET_NOARG;
	}

	if (is_number(argv[2]) == false) {
		printf("\"%s\" is not a number!\n", argv[2]);
		return RET_BADVAL;
	}

	float value;
	float brightness = 100;

	value = atof(argv[2]);
	
	if (!read_brightness(CFGFILE, brightness)) {
		printf("Using default brightness value %.1f.\n", brightness);
	}

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
		printf("Bad argument \"%s\"\n", argv[1]);
		return RET_BADOP;
	}

	write_brightness(CFGFILE, brightness, true);
	write_brightness(SYSFILE, brightness, false);

	return 0;
}

bool read_brightness(const char* filename, float& brightness) {
	FILE *f = fopen(filename, "r");

	if (f == NULL) {
		printf("Failed to open file %s for reading!\n", filename);
		return false;
	}

	fscanf(f, "%f", &brightness);
	fclose(f);

	return true;
}

void write_brightness(const char* filename, float brightness, bool write_as_float) {
	FILE *f = fopen(filename, "w");
	float max_brightness;

	read_brightness(SYSMAXFILE, max_brightness);

	if (f == NULL) {
		printf("Failed to open file %s for writing!\n", filename);
	}

	char format_string[6] = "%.0f\n";

	if (write_as_float == true) {
		strcpy(format_string, "%f\n");
	}

	if (brightness < max_brightness && brightness > MIN_BRIGHTNESS) {
		fprintf(f, format_string, brightness);
	} else if (brightness > max_brightness) {
		fprintf(f, format_string, max_brightness);
	} else if (brightness < MIN_BRIGHTNESS) {
		fprintf(f, format_string, MIN_BRIGHTNESS);
	}

	fclose(f);
}

bool is_number(const char* str) {
	bool result = true;

	for (int i = 0; i < strlen(str); i++) {
		if ((str[i] < 48 || str[i] > 57) && str[i] != 46) {
			result = false;
		}
	}

	return result;
}

