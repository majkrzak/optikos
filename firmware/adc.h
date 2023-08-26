#pragma once

#include <stdint.h>

#include <stddef.h>

/// Initialize the Analog to Digital Converter
void adc_init();

/// Selects Analog to Digital Converter MUX
void adc_select(size_t idx);

/// Read value out of the Analog to Digital Converter
/**
 *  Reads value out of the `idx`'th analog pin into the `res`.
 *  Arduino Leonardo pin numbering is used for `idx`
 */
void adc_read(uint8_t *res);
