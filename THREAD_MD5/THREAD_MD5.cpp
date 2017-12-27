#define _CRT_SECURE_NO_WARNINGS
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <random>

#define SIZEMD5 16

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
    (a) += F ((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G ((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H ((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
  }

char pad_buffer[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

typedef struct {
	uint32_t state[4];
	uint32_t count[2];
	char buffer[64];
} context_t, *pcontext_t;

void init(pcontext_t ctx) {
	assert(ctx);
	ctx->count[0] = 0;
	ctx->count[1] = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xefcdab89;
	ctx->state[2] = 0x98badcfe;
	ctx->state[3] = 0x10325476;
}

void encode(char *out, uint32_t *in, size_t len) {
	size_t i, j;

	assert(out && in);
	for (i = 0, j = 0; j < len; ++i, j += 4) {
		out[j] = (char)(in[i] & 0xff);
		out[j + 1] = (char)((in[i] >> 8) & 0xff);
		out[j + 2] = (char)((in[i] >> 16) & 0xff);
		out[j + 3] = (char)((in[i] >> 24) & 0xff);
	}
}

void decode(uint32_t *out, char *in, size_t len) {
	size_t i, j;

	assert(out && in);
	for (i = 0, j = 0; j < len; ++i, j += 4)
		out[i] = ((uint32_t)in[j] | (((uint32_t)in[j + 1]) << 8) |
		(((uint32_t)in[j + 2]) << 16) | (((uint32_t)in[j + 3]) << 24));
}

void transform(uint32_t *state, char *block) {
	uint32_t a = state[0];
	uint32_t b = state[1];
	uint32_t c = state[2];
	uint32_t d = state[3];
	uint32_t x[16];

	assert(state && block);
	decode(x, block, 64);

	/* Round 1 */
	FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
	FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
	FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
	FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
	FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
	FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
	FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
	FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
	FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
	FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

											/* Round 2 */
	GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
	GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
	GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
	GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
	GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
	GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
	GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
	GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
	GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
	GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

											/* Round 3 */
	HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
	HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
	HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
	HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
	HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
	HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

										   /* Round 4 */
	II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
	II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
	II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
	II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
	II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
	II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
	II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
	II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	memset(x, 0, sizeof(x));
}

void update(pcontext_t ctx, char *in, size_t len) {
	size_t i = 0;
	size_t index = 0;
	size_t part_len = 0;

	assert(ctx);

	index = (size_t)((ctx->count[0] >> 3) & 0x3f);
	if ((ctx->count[0] += ((uint32_t)len << 3)) < ((uint32_t)len << 3))
		++ctx->count[1];

	ctx->count[1] += ((uint32_t)len >> 29);
	part_len = 64 - index;

	if (len >= part_len) {
		memcpy(&ctx->buffer[index], in, part_len);
		transform(ctx->state, ctx->buffer);

		for (i = part_len; i + 63 < len; i += 64)
			transform(ctx->state, &in[i]);

		index = 0;
	}
	else {
		i = 0;
	}

	memcpy(&ctx->buffer[index], &in[i], len - i);
}

void final(char digest[16], pcontext_t ctx) {
	char bits[8];
	size_t index = 0;
	size_t pad_len = 0;

	assert(ctx);

	encode(bits, ctx->count, 8);

	index = (size_t)((ctx->count[0] >> 3) & 0x3f);
	pad_len = (index < 56) ? (56 - index) : (120 - index);

	update(ctx, pad_buffer, pad_len);
	update(ctx, bits, 8);

	encode(digest, ctx->state, 16);
	memset(ctx, 0, sizeof(*ctx));
}

int md5string(char *string, int length, char *digest)
{
	context_t ctx;
	init(&ctx);
	update(&ctx, string, length);
	final(digest, &ctx);

	return 1;
}

void start(char *correct_md5, char* correct_pass, int id)
{
	char *alph = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	char str[3];
	char md5[SIZEMD5];

	str[0] = alph[id % 62];
	str[1] = alph[(id / 62) % 62];
	str[2] = alph[(id / 62 / 62) % 62];

	md5string(str, 3, md5);
	if (!memcmp(correct_md5, md5, SIZEMD5))	for (int i = 0; i < 3; i++) correct_pass[i] = str[i];
	
}

void md5main(int id)
{
	char *str = "Hel";
	char *md5 = new char[SIZEMD5];

	md5string(str, strlen(str), md5);

	/*for (int i = 0; i < 16; i++)
		printf("%02hhx", md5[i]);
	printf("\n");*/

	char *newStr = new char[strlen(str)];

	start(md5, newStr, id);
	if (!memcmp(str, newStr, 3)) std::cout << "str = " << str << "\tnewStr = " << newStr << std::endl;;
}

int main(int argc, char **argv)
{
	std::vector<std::thread> threadVector;
	for (int i = 0; i < 62 * 62 * 62; i++)
	{
		std::thread th(md5main, i);
		threadVector.push_back(move(th));
	}
	std::cout << "Hi\n";
	for (auto it = threadVector.begin(); it != threadVector.end(); it++)
	{
		(*it).join();
	}
	system("pause");
	return 0;
}
