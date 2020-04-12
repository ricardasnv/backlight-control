#ifndef CONFIG_H
#define CONFIG_H

#define SYSFILE "/sys/class/backlight/intel_backlight/brightness"        // System brightness file
#define SYSMAXFILE "/sys/class/backlight/intel_backlight/max_brightness" // System max brightness file

#define BRDIR "/.backlight/"            // Brightness file directory
#define DEFBRFILE "/.backlight/default" // Default brightness
#define CURBRFILE "/.backlight/current" // Current brightness

// Minimum allowable brightness
// (best to set it at >0 so that brightness level doesn't get stuck at zero
//  when only mul/div is used)
#define MIN_BRIGHTNESS 0.5

#endif
