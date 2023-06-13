#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

char* base64_encode(const uint8_t* data, size_t input_length);

int main() {
    srand(time(NULL));

    int randomPrefix;
    int num = (int)(4294967296 * ((double)rand() / RAND_MAX));
    time_t creationTime = time(NULL) * 1000;
    
    if (num <= 2147483647) {
        randomPrefix = num;
    } else {
        randomPrefix = num - 4294967296;
    }

    int userID = atoi("USER ID HERE");

    int firstTemp = (userID % 4294967296 <= 2147483647) ? (userID % 4294967296) : (userID % 4294967296 - 2147483647);
    uint8_t first[4];
    *((int32_t*)first) = (uint32_t)firstTemp;

    uint8_t second[4];
    *((int32_t*)second) = (uint32_t)(userID >> 32);

    uint8_t third[4];
    *((int32_t*)third) = (uint32_t)randomPrefix;

    int64_t fourthTemp = (creationTime % 4294967296 <= 2147483647) ? (creationTime % 4294967296) : (creationTime % 4294967296 - 2147483647);
    uint8_t fourth[4];
    *((int32_t*)fourth) = (uint32_t)fourthTemp;

    uint8_t fifth[4];
    *((int32_t*)fifth) = (uint32_t)(creationTime >> 32);

    uint8_t sixth[4];
    *((int32_t*)sixth) = 0;

    uint8_t buf[24];
    memcpy(buf, first, 4);
    memcpy(buf + 4, second, 4);
    memcpy(buf + 8, third, 4);
    memcpy(buf + 12, fourth, 4);
    memcpy(buf + 16, fifth, 4);
    memcpy(buf + 20, sixth, 4);

    char* encoded = base64_encode(buf, sizeof(buf));
    printf("%s\n", encoded);

    free(encoded);

    return 0;
}

char* base64_encode(const uint8_t* data, size_t input_length) {
    const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    size_t output_length = 4 * ((input_length + 2) / 3);
    char* encoded_data = (char*)malloc(output_length + 1);

    if (encoded_data == NULL) {
        return NULL;
    }

    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? (uint8_t)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (uint8_t)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (uint8_t)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = base64_chars[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = base64_chars[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = base64_chars[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = base64_chars[(triple >> 0 * 6) & 0x3F];
    }

    for (size_t i = 0; i < (3 - input_length % 3) % 3; i++) {
        encoded_data[output_length - 1 - i] = '=';
    }

    encoded_data[output_length] = '\0';

    return encoded_data;
}
