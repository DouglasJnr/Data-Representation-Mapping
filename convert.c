#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

// Function prototypes
void oct_to_bin(const char *oct, char *out);
void oct_to_hex(const char *oct, char *out);
void hex_to_bin(const char *hex, char *out);
void to_sign_magnitude(int32_t n, char *out);
void to_ones_complement(int32_t n, char *out);
void to_twos_complement(int32_t n, char *out);

// Helper function to convert 32-bit value to binary string
void to_32bit_binary(uint32_t value, char *out) {
    for (int i = 31; i >= 0; i--) {
        out[31 - i] = ((value >> i) & 1) ? '1' : '0';
    }
    out[32] = '\0';
}

// Function 1: Convert octal to binary using 3-bit mapping
void oct_to_bin(const char *oct, char *out) {
    int pos = 0;
    int len = strlen(oct);
    
    for (int i = 0; i < len; i++) {
        int octal_digit = oct[i] - '0';
        
        // Map each octal digit to 3 binary digits
        out[pos++] = (octal_digit >> 2) & 1 ? '1' : '0';
        out[pos++] = (octal_digit >> 1) & 1 ? '1' : '0';
        out[pos++] = (octal_digit >> 0) & 1 ? '1' : '0';
    }
    
    out[pos] = '\0';
}

// Function 2: Convert octal to hexadecimal via binary
void oct_to_hex(const char *oct, char *out) {
    char binary[1000];
    oct_to_bin(oct, binary);
    
    int bin_len = strlen(binary);
    int pos = 0;
    
    // Pad binary to multiple of 4 bits
    int padding = (4 - (bin_len % 4)) % 4;
    char padded_binary[1000];
    
    for (int i = 0; i < padding; i++) {
        padded_binary[i] = '0';
    }
    strcpy(padded_binary + padding, binary);
    
    bin_len += padding;
    
    // Convert groups of 4 bits to hex
    for (int i = 0; i < bin_len; i += 4) {
        int hex_val = 0;
        for (int j = 0; j < 4; j++) {
            if (padded_binary[i + j] == '1') {
                hex_val |= (1 << (3 - j));
            }
        }
        
        if (hex_val < 10) {
            out[pos++] = '0' + hex_val;
        } else {
            out[pos++] = 'A' + (hex_val - 10);
        }
    }
    
    // Remove leading zeros (except if result is single "0")
    int start = 0;
    while (start < pos - 1 && out[start] == '0') {
        start++;
    }
    
    if (start > 0) {
        for (int i = 0; i < pos - start; i++) {
            out[i] = out[start + i];
        }
        pos -= start;
    }
    
    out[pos] = '\0';
}

// Function 3: Convert hexadecimal to binary using 4-bit mapping
void hex_to_bin(const char *hex, char *out) {
    int pos = 0;
    int len = strlen(hex);
    
    for (int i = 0; i < len; i++) {
        char c = hex[i];
        int hex_val;
        
        // Convert hex character to value
        if (c >= '0' && c <= '9') {
            hex_val = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            hex_val = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            hex_val = c - 'a' + 10;
        } else {
            continue; // Skip invalid characters
        }
        
        // Map each hex digit to 4 binary digits
        out[pos++] = (hex_val >> 3) & 1 ? '1' : '0';
        out[pos++] = (hex_val >> 2) & 1 ? '1' : '0';
        out[pos++] = (hex_val >> 1) & 1 ? '1' : '0';
        out[pos++] = (hex_val >> 0) & 1 ? '1' : '0';
    }
    
    // Don't remove leading zeros - keep exact 4-bit mapping per digit
    out[pos] = '\0';
}

// Function 4: Convert to sign-magnitude representation
void to_sign_magnitude(int32_t n, char *out) {
    if (n >= 0) {
        // Positive number: regular binary with 32 bits
        to_32bit_binary((uint32_t)n, out);
    } else {
        // Negative number: set bit 31 to 1, keep magnitude in bits 0-30
        uint32_t magnitude = (uint32_t)(-n);
        magnitude |= 0x80000000; // Set sign bit (bit 31)
        to_32bit_binary(magnitude, out);
    }
}

// Function 5: Convert to one's complement representation
void to_ones_complement(int32_t n, char *out) {
    if (n >= 0) {
        // Positive number: regular binary with 32 bits
        to_32bit_binary((uint32_t)n, out);
    } else {
        // Negative number: flip all bits
        uint32_t positive = (uint32_t)(-n);
        uint32_t ones_comp = ~positive; // Flip all bits
        to_32bit_binary(ones_comp, out);
    }
}

// Function 6: Convert to two's complement representation
void to_twos_complement(int32_t n, char *out) {
    // Two's complement: this is how modern CPUs store integers
    // The bit pattern IS the two's complement representation
    uint32_t bit_pattern = (uint32_t)n;
    to_32bit_binary(bit_pattern, out);
}
