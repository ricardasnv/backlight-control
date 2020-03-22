#ifndef CONFIG_H
#define CONFIG_H

// Control files
#define SYSFILE "/sys/class/backlight/intel_backlight/brightness"
#define SYSMAXFILE "/sys/class/backlight/intel_backlight/max_brightness"

// Default and current brightness files
#define DEFBRFILE "/.backlight/default"
#define CURBRFILE "/.backlight/current"

// Minimum allowable brightness
// (best to set it at >0 so that brightness level doesn't get stuck at zero
//  when only mul/div is used)
#define MIN_BRIGHTNESS 0.5

#endif
