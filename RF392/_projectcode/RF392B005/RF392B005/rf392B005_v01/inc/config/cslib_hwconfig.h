//=============================================================================
// inc/config/cslib_hwconfig.h: generated by Hardware Configurator
//
// This file will be regenerated when saving a document. leave the sections
// inside the "$[...]" comment tags alone or they will be overwritten!
//=============================================================================

#ifndef __SILICON_LABS_CSLIB_HWCONFIG_H
#define __SILICON_LABS_CSLIB_HWCONFIG_H

// $[mux values]
#define MUX_VALUE_ARRAY \
  0x01,     /* CS0.1 */ \
  0x00,     /* CS0.0 */ \
  0x02,     /* CS0.2 */ \
  0x03,     /* CS0.3 */ \
  0x07,     /* CS0.7 */ \
  0x08,     /* CS1.0 */ \
  0x09,     /* CS1.1 */ \
// [mux values]$

// $[gain values]
#define GAIN_VALUE_ARRAY \
  0x07,     /* CS0.1 */ \
  0x07,     /* CS0.0 */ \
  0x07,     /* CS0.2 */ \
  0x07,     /* CS0.3 */ \
  0x07,     /* CS0.7 */ \
  0x07,     /* CS1.0 */ \
  0x07,     /* CS1.1 */ \
// [gain values]$

// $[accumulation values]
#define ACCUMULATION_VALUE_ARRAY \
  0x02,     /* CS0.1 */ \
  0x02,     /* CS0.0 */ \
  0x02,     /* CS0.2 */ \
  0x02,     /* CS0.3 */ \
  0x02,     /* CS0.7 */ \
  0x02,     /* CS1.0 */ \
  0x02,     /* CS1.1 */ \
// [accumulation values]$

// $[active threshold values]
#define ACTIVE_THRESHOLD_ARRAY \
  57,       /* CS0.1 */ \
  55,       /* CS0.0 */ \
  57,       /* CS0.2 */ \
  57,       /* CS0.3 */ \
  57,       /* CS0.7 */ \
  57,       /* CS1.0 */ \
  55,       /* CS1.1 */ \
// [active threshold values]$

// $[inactive threshold values]
#define INACTIVE_THRESHOLD_ARRAY \
  45,       /* CS0.1 */ \
  45,       /* CS0.0 */ \
  45,       /* CS0.2 */ \
  45,       /* CS0.3 */ \
  45,       /* CS0.7 */ \
  45,       /* CS1.0 */ \
  45,       /* CS1.1 */ \
// [inactive threshold values]$

// $[average touch delta values]
#define AVERAGE_TOUCH_DELTA_ARRAY \
  400>>4,   /* CS0.1 */ \
  400>>4,   /* CS0.0 */ \
  400>>4,   /* CS0.2 */ \
  500>>4,   /* CS0.3 */ \
  400>>4,   /* CS0.7 */ \
  400>>4,   /* CS1.0 */ \
  400>>4,   /* CS1.1 */ \
// [average touch delta values]$

// $[Active mode mask]
#define ACTIVE_MODE_MASK_P0   0x8F
#define ACTIVE_MODE_MASK_P1   0x03
// [Active mode mask]$

// $[Sleep mode mask]
#define SLEEP_MODE_MASK_P0    0x8F
#define SLEEP_MODE_MASK_P1    0x03
// [Sleep mode mask]$

#endif // __SILICON_LABS_CSLIB_HWCONFIG_H

