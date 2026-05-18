// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/hmac.h>

#include <reymon/reymm/reymm.h>
#include <reymon/reymm/connect/binance.h>


void reymm_signature(char* signature, const char* payload) {

	unsigned char digest[32]; // SHA256 = 32 bytes
	unsigned int  digest_len = sizeof(digest);

	HMAC(
		EVP_sha256(),
		setting.apisecret,
		(int) strlen(setting.apisecret),
		(unsigned char*) payload,
		strlen(payload),
		digest,
		&digest_len
	);

	// Convertir bytes a hex string
	for (size_t i = 0; i < digest_len; i++) {
		sprintf(&signature[i * 2], "%02x", digest[i]);
	}

	signature[digest_len * 2] = '\0';
}