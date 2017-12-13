/*
 * Copyright (c) 2017, Linaro Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sks_abi.h>
#include <sks_ta.h>
#include <stdio.h>
#include <string.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include "ck_debug.h"

#define CK2STR_ENTRY(label)	{ .id = label, .string = #label }

struct ck2str {
	CK_ULONG id;
	const char *string;
};

/*
 * Return a pointer to a string buffer of "CKA_xxx\0" attribute ID
 */
static struct ck2str cka2str_table[] = {
	/* OP-TEE SKS ID representing a not-yet defined value */
	CK2STR_ENTRY(CK_VENDOR_UNDEFINED_ID),
	CK2STR_ENTRY(CKA_SKS_SECSTOR_DATA),
	/* Standard CK attributes */
	CK2STR_ENTRY(CKA_CLASS),
	CK2STR_ENTRY(CKA_TOKEN),
	CK2STR_ENTRY(CKA_PRIVATE),
	CK2STR_ENTRY(CKA_LABEL),
	CK2STR_ENTRY(CKA_APPLICATION),
	CK2STR_ENTRY(CKA_VALUE),
	CK2STR_ENTRY(CKA_OBJECT_ID),
	CK2STR_ENTRY(CKA_CERTIFICATE_TYPE),
	CK2STR_ENTRY(CKA_ISSUER),
	CK2STR_ENTRY(CKA_SERIAL_NUMBER),
	CK2STR_ENTRY(CKA_AC_ISSUER),
	CK2STR_ENTRY(CKA_OWNER),
	CK2STR_ENTRY(CKA_ATTR_TYPES),
	CK2STR_ENTRY(CKA_TRUSTED),
	CK2STR_ENTRY(CKA_CERTIFICATE_CATEGORY),
	CK2STR_ENTRY(CKA_JAVA_MIDP_SECURITY_DOMAIN),
	CK2STR_ENTRY(CKA_URL),
	CK2STR_ENTRY(CKA_HASH_OF_SUBJECT_PUBLIC_KEY),
	CK2STR_ENTRY(CKA_HASH_OF_ISSUER_PUBLIC_KEY),
	CK2STR_ENTRY(CKA_NAME_HASH_ALGORITHM),
	CK2STR_ENTRY(CKA_CHECK_VALUE),
	CK2STR_ENTRY(CKA_KEY_TYPE),
	CK2STR_ENTRY(CKA_SUBJECT),
	CK2STR_ENTRY(CKA_ID),
	CK2STR_ENTRY(CKA_SENSITIVE),
	CK2STR_ENTRY(CKA_ENCRYPT),
	CK2STR_ENTRY(CKA_DECRYPT),
	CK2STR_ENTRY(CKA_WRAP),
	CK2STR_ENTRY(CKA_UNWRAP),
	CK2STR_ENTRY(CKA_SIGN),
	CK2STR_ENTRY(CKA_SIGN_RECOVER),
	CK2STR_ENTRY(CKA_VERIFY),
	CK2STR_ENTRY(CKA_VERIFY_RECOVER),
	CK2STR_ENTRY(CKA_DERIVE),
	CK2STR_ENTRY(CKA_START_DATE),
	CK2STR_ENTRY(CKA_END_DATE),
	CK2STR_ENTRY(CKA_MODULUS),
	CK2STR_ENTRY(CKA_MODULUS_BITS),
	CK2STR_ENTRY(CKA_PUBLIC_EXPONENT),
	CK2STR_ENTRY(CKA_PRIVATE_EXPONENT),
	CK2STR_ENTRY(CKA_PRIME_1),
	CK2STR_ENTRY(CKA_PRIME_2),
	CK2STR_ENTRY(CKA_EXPONENT_1),
	CK2STR_ENTRY(CKA_EXPONENT_2),
	CK2STR_ENTRY(CKA_COEFFICIENT),
	CK2STR_ENTRY(CKA_PUBLIC_KEY_INFO),
	CK2STR_ENTRY(CKA_PRIME),
	CK2STR_ENTRY(CKA_SUBPRIME),
	CK2STR_ENTRY(CKA_BASE),
	CK2STR_ENTRY(CKA_PRIME_BITS),
	CK2STR_ENTRY(CKA_SUBPRIME_BITS),
	CK2STR_ENTRY(CKA_VALUE_BITS),
	CK2STR_ENTRY(CKA_VALUE_LEN),
	CK2STR_ENTRY(CKA_EXTRACTABLE),
	CK2STR_ENTRY(CKA_LOCAL),
	CK2STR_ENTRY(CKA_NEVER_EXTRACTABLE),
	CK2STR_ENTRY(CKA_ALWAYS_SENSITIVE),
	CK2STR_ENTRY(CKA_KEY_GEN_MECHANISM),
	CK2STR_ENTRY(CKA_MODIFIABLE),
	CK2STR_ENTRY(CKA_COPYABLE),
	CK2STR_ENTRY(CKA_DESTROYABLE),
	CK2STR_ENTRY(CKA_EC_PARAMS),
	CK2STR_ENTRY(CKA_EC_POINT),
	CK2STR_ENTRY(CKA_SECONDARY_AUTH),
	CK2STR_ENTRY(CKA_AUTH_PIN_FLAGS),
	CK2STR_ENTRY(CKA_ALWAYS_AUTHENTICATE),
	CK2STR_ENTRY(CKA_WRAP_WITH_TRUSTED),
	CK2STR_ENTRY(CKA_WRAP_TEMPLATE),
	CK2STR_ENTRY(CKA_UNWRAP_TEMPLATE),
	CK2STR_ENTRY(CKA_DERIVE_TEMPLATE),
	CK2STR_ENTRY(CKA_OTP_FORMAT),
	CK2STR_ENTRY(CKA_OTP_LENGTH),
	CK2STR_ENTRY(CKA_OTP_TIME_INTERVAL),
	CK2STR_ENTRY(CKA_OTP_USER_FRIENDLY_MODE),
	CK2STR_ENTRY(CKA_OTP_CHALLENGE_REQUIREMENT),
	CK2STR_ENTRY(CKA_OTP_TIME_REQUIREMENT),
	CK2STR_ENTRY(CKA_OTP_COUNTER_REQUIREMENT),
	CK2STR_ENTRY(CKA_OTP_PIN_REQUIREMENT),
	CK2STR_ENTRY(CKA_OTP_COUNTER),
	CK2STR_ENTRY(CKA_OTP_TIME),
	CK2STR_ENTRY(CKA_OTP_USER_IDENTIFIER),
	CK2STR_ENTRY(CKA_OTP_SERVICE_IDENTIFIER),
	CK2STR_ENTRY(CKA_OTP_SERVICE_LOGO),
	CK2STR_ENTRY(CKA_OTP_SERVICE_LOGO_TYPE),
	CK2STR_ENTRY(CKA_GOSTR3410_PARAMS),
	CK2STR_ENTRY(CKA_GOSTR3411_PARAMS),
	CK2STR_ENTRY(CKA_GOST28147_PARAMS),
	CK2STR_ENTRY(CKA_HW_FEATURE_TYPE),
	CK2STR_ENTRY(CKA_RESET_ON_INIT),
	CK2STR_ENTRY(CKA_HAS_RESET),
	CK2STR_ENTRY(CKA_PIXEL_X),
	CK2STR_ENTRY(CKA_PIXEL_Y),
	CK2STR_ENTRY(CKA_RESOLUTION),
	CK2STR_ENTRY(CKA_CHAR_ROWS),
	CK2STR_ENTRY(CKA_CHAR_COLUMNS),
	CK2STR_ENTRY(CKA_COLOR),
	CK2STR_ENTRY(CKA_BITS_PER_PIXEL),
	CK2STR_ENTRY(CKA_CHAR_SETS),
	CK2STR_ENTRY(CKA_ENCODING_METHODS),
	CK2STR_ENTRY(CKA_MIME_TYPES),
	CK2STR_ENTRY(CKA_MECHANISM_TYPE),
	CK2STR_ENTRY(CKA_REQUIRED_CMS_ATTRIBUTES),
	CK2STR_ENTRY(CKA_DEFAULT_CMS_ATTRIBUTES),
	CK2STR_ENTRY(CKA_SUPPORTED_CMS_ATTRIBUTES),
	CK2STR_ENTRY(CKA_ALLOWED_MECHANISMS),
	CK2STR_ENTRY(CKA_VENDOR_DEFINED),
};

const char *cka2str(CK_ATTRIBUTE_TYPE id)
{
	static const char vendor_range[] = "<unknwon-vendor-defined>";
	static const char unknown[] = "<unknown-identifier>>";
	const int count = sizeof(cka2str_table) / sizeof(struct ck2str);
	int n;

	for (n = 0; n < count; n++) {
		if (id == cka2str_table[n].id)
			return cka2str_table[n].string;
	}

	if (id > CKA_VENDOR_DEFINED)
		return vendor_range;

	return unknown;
}

/*
 * ckr2str - Return a pointer to a string buffer of "CKR_xxx\0" attribute ID
 */

static struct ck2str ckr2str_table[] = {
	CK2STR_ENTRY(CKR_OK),
	CK2STR_ENTRY(CKR_CANCEL),
	CK2STR_ENTRY(CKR_HOST_MEMORY),
	CK2STR_ENTRY(CKR_SLOT_ID_INVALID),
	CK2STR_ENTRY(CKR_GENERAL_ERROR),
	CK2STR_ENTRY(CKR_FUNCTION_FAILED),
	CK2STR_ENTRY(CKR_ARGUMENTS_BAD),
	CK2STR_ENTRY(CKR_NO_EVENT),
	CK2STR_ENTRY(CKR_NEED_TO_CREATE_THREADS),
	CK2STR_ENTRY(CKR_CANT_LOCK),
	CK2STR_ENTRY(CKR_ATTRIBUTE_READ_ONLY),
	CK2STR_ENTRY(CKR_ATTRIBUTE_SENSITIVE),
	CK2STR_ENTRY(CKR_ATTRIBUTE_TYPE_INVALID),
	CK2STR_ENTRY(CKR_ATTRIBUTE_VALUE_INVALID),
	CK2STR_ENTRY(CKR_ACTION_PROHIBITED),
	CK2STR_ENTRY(CKR_DATA_INVALID),
	CK2STR_ENTRY(CKR_DATA_LEN_RANGE),
	CK2STR_ENTRY(CKR_DEVICE_ERROR),
	CK2STR_ENTRY(CKR_DEVICE_MEMORY),
	CK2STR_ENTRY(CKR_DEVICE_REMOVED),
	CK2STR_ENTRY(CKR_ENCRYPTED_DATA_INVALID),
	CK2STR_ENTRY(CKR_ENCRYPTED_DATA_LEN_RANGE),
	CK2STR_ENTRY(CKR_FUNCTION_CANCELED),
	CK2STR_ENTRY(CKR_FUNCTION_NOT_PARALLEL),
	CK2STR_ENTRY(CKR_FUNCTION_NOT_SUPPORTED),
	CK2STR_ENTRY(CKR_KEY_HANDLE_INVALID),
	CK2STR_ENTRY(CKR_KEY_SIZE_RANGE),
	CK2STR_ENTRY(CKR_KEY_TYPE_INCONSISTENT),
	CK2STR_ENTRY(CKR_KEY_NOT_NEEDED),
	CK2STR_ENTRY(CKR_KEY_CHANGED),
	CK2STR_ENTRY(CKR_KEY_NEEDED),
	CK2STR_ENTRY(CKR_KEY_INDIGESTIBLE),
	CK2STR_ENTRY(CKR_KEY_FUNCTION_NOT_PERMITTED),
	CK2STR_ENTRY(CKR_KEY_NOT_WRAPPABLE),
	CK2STR_ENTRY(CKR_KEY_UNEXTRACTABLE),
	CK2STR_ENTRY(CKR_MECHANISM_INVALID),
	CK2STR_ENTRY(CKR_MECHANISM_PARAM_INVALID),
	CK2STR_ENTRY(CKR_OBJECT_HANDLE_INVALID),
	CK2STR_ENTRY(CKR_OPERATION_ACTIVE),
	CK2STR_ENTRY(CKR_OPERATION_NOT_INITIALIZED),
	CK2STR_ENTRY(CKR_PIN_INCORRECT),
	CK2STR_ENTRY(CKR_PIN_INVALID),
	CK2STR_ENTRY(CKR_PIN_LEN_RANGE),
	CK2STR_ENTRY(CKR_PIN_EXPIRED),
	CK2STR_ENTRY(CKR_PIN_LOCKED),
	CK2STR_ENTRY(CKR_SESSION_CLOSED),
	CK2STR_ENTRY(CKR_SESSION_COUNT),
	CK2STR_ENTRY(CKR_SESSION_HANDLE_INVALID),
	CK2STR_ENTRY(CKR_SESSION_PARALLEL_NOT_SUPPORTED),
	CK2STR_ENTRY(CKR_SESSION_READ_ONLY),
	CK2STR_ENTRY(CKR_SESSION_EXISTS),
	CK2STR_ENTRY(CKR_SESSION_READ_ONLY_EXISTS),
	CK2STR_ENTRY(CKR_SESSION_READ_WRITE_SO_EXISTS),
	CK2STR_ENTRY(CKR_SIGNATURE_INVALID),
	CK2STR_ENTRY(CKR_SIGNATURE_LEN_RANGE),
	CK2STR_ENTRY(CKR_TEMPLATE_INCOMPLETE),
	CK2STR_ENTRY(CKR_TEMPLATE_INCONSISTENT),
	CK2STR_ENTRY(CKR_TOKEN_NOT_PRESENT),
	CK2STR_ENTRY(CKR_TOKEN_NOT_RECOGNIZED),
	CK2STR_ENTRY(CKR_TOKEN_WRITE_PROTECTED),
	CK2STR_ENTRY(CKR_UNWRAPPING_KEY_HANDLE_INVALID),
	CK2STR_ENTRY(CKR_UNWRAPPING_KEY_SIZE_RANGE),
	CK2STR_ENTRY(CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT),
	CK2STR_ENTRY(CKR_USER_ALREADY_LOGGED_IN),
	CK2STR_ENTRY(CKR_USER_NOT_LOGGED_IN),
	CK2STR_ENTRY(CKR_USER_PIN_NOT_INITIALIZED),
	CK2STR_ENTRY(CKR_USER_TYPE_INVALID),
	CK2STR_ENTRY(CKR_USER_ANOTHER_ALREADY_LOGGED_IN),
	CK2STR_ENTRY(CKR_USER_TOO_MANY_TYPES),
	CK2STR_ENTRY(CKR_WRAPPED_KEY_INVALID),
	CK2STR_ENTRY(CKR_WRAPPED_KEY_LEN_RANGE),
	CK2STR_ENTRY(CKR_WRAPPING_KEY_HANDLE_INVALID),
	CK2STR_ENTRY(CKR_WRAPPING_KEY_SIZE_RANGE),
	CK2STR_ENTRY(CKR_WRAPPING_KEY_TYPE_INCONSISTENT),
	CK2STR_ENTRY(CKR_RANDOM_SEED_NOT_SUPPORTED),
	CK2STR_ENTRY(CKR_RANDOM_NO_RNG),
	CK2STR_ENTRY(CKR_DOMAIN_PARAMS_INVALID),
	CK2STR_ENTRY(CKR_CURVE_NOT_SUPPORTED),
	CK2STR_ENTRY(CKR_BUFFER_TOO_SMALL),
	CK2STR_ENTRY(CKR_SAVED_STATE_INVALID),
	CK2STR_ENTRY(CKR_INFORMATION_SENSITIVE),
	CK2STR_ENTRY(CKR_STATE_UNSAVEABLE),
	CK2STR_ENTRY(CKR_CRYPTOKI_NOT_INITIALIZED),
	CK2STR_ENTRY(CKR_CRYPTOKI_ALREADY_INITIALIZED),
	CK2STR_ENTRY(CKR_MUTEX_BAD),
	CK2STR_ENTRY(CKR_MUTEX_NOT_LOCKED),
	CK2STR_ENTRY(CKR_NEW_PIN_MODE),
	CK2STR_ENTRY(CKR_NEXT_OTP),
	CK2STR_ENTRY(CKR_EXCEEDED_MAX_ITERATIONS),
	CK2STR_ENTRY(CKR_FIPS_SELF_TEST_FAILED),
	CK2STR_ENTRY(CKR_LIBRARY_LOAD_FAILED),
	CK2STR_ENTRY(CKR_PIN_TOO_WEAK),
	CK2STR_ENTRY(CKR_PUBLIC_KEY_INVALID),
	CK2STR_ENTRY(CKR_FUNCTION_REJECTED),
	CK2STR_ENTRY(CKR_VENDOR_DEFINED),
};

const char *ckr2str(CK_RV id)
{
	static const char vendor[] = "(vendor-defined)";
	static const char unknown[] = "(unknown)";
	const int count = sizeof(ckr2str_table) / sizeof(struct ck2str);
	int n;

	for (n = 0; n < count; n++) {
		if (id == ckr2str_table[n].id)
			return ckr2str_table[n].string;
	}

	if (id >= CKR_VENDOR_DEFINED)
		return vendor;

	return unknown;
}

TEE_Result ckr2tee(CK_RV rv)
{
	switch (rv) {
	case CKR_OK:
		return TEE_SUCCESS;
	case CKR_ARGUMENTS_BAD:
		return TEE_ERROR_BAD_PARAMETERS;
	case CKR_DEVICE_MEMORY:
		return TEE_ERROR_OUT_OF_MEMORY;
	case CKR_BUFFER_TOO_SMALL:
		return TEE_ERROR_SHORT_BUFFER;
	default:
		return TEE_ERROR_GENERIC;
	}
}

static struct ck2str skscmd2str_table[] = {
	CK2STR_ENTRY(SKS_CMD_CK_PING),
	CK2STR_ENTRY(SKS_CMD_CK_SLOT_LIST),
	CK2STR_ENTRY(SKS_CMD_CK_SLOT_INFO),
	CK2STR_ENTRY(SKS_CMD_CK_TOKEN_INFO),
	CK2STR_ENTRY(SKS_CMD_CK_MECHANISM_IDS),
	CK2STR_ENTRY(SKS_CMD_CK_MECHANISM_INFO),
	CK2STR_ENTRY(SKS_CMD_CK_INIT_TOKEN),
	CK2STR_ENTRY(SKS_CMD_CK_INIT_PIN),
	CK2STR_ENTRY(SKS_CMD_CK_SET_PIN),
	CK2STR_ENTRY(SKS_CMD_CK_OPEN_RO_SESSION),
	CK2STR_ENTRY(SKS_CMD_CK_OPEN_RW_SESSION),
	CK2STR_ENTRY(SKS_CMD_CK_CLOSE_SESSION),
	CK2STR_ENTRY(SKS_CMD_CK_SESSION_INFO),
	CK2STR_ENTRY(SKS_CMD_CK_CREATE_OBJECT),
	CK2STR_ENTRY(SKS_CMD_CK_DESTROY_OBJECT),
	CK2STR_ENTRY(SKS_CMD_CK_ENCRYPT_INIT),
	CK2STR_ENTRY(SKS_CMD_CK_DECRYPT_INIT),
	CK2STR_ENTRY(SKS_CMD_CK_ENCRYPT_UPDATE),
	CK2STR_ENTRY(SKS_CMD_CK_DECRYPT_UPDATE),
	CK2STR_ENTRY(SKS_CMD_CK_DECRYPT_FINAL),
	CK2STR_ENTRY(SKS_CMD_CK_ENCRYPT_FINAL),
};

const char *skscmd2str(uint32_t id)
{
	static const char unknown[] = "<invalid-command-id>";
	const int count = sizeof(skscmd2str_table) / sizeof(struct ck2str);
	int n;

	for (n = 0; n < count; n++) {
		if (id == skscmd2str_table[n].id)
			return skscmd2str_table[n].string;
	}

	return unknown;

}

static struct ck2str slotflag2str_table[] = {
	CK2STR_ENTRY(CKF_TOKEN_PRESENT),
	CK2STR_ENTRY(CKF_REMOVABLE_DEVICE),
	CK2STR_ENTRY(CKF_HW_SLOT),
};

static const char *slot_flags2str(CK_ULONG flags)
{
	const int count = sizeof(slotflag2str_table) / sizeof(struct ck2str);
	int n;

	for (n = 0; n < count; n++) {
		if (flags & slotflag2str_table[n].id)
			return slotflag2str_table[n].string;
	}

	return NULL;
}

static struct ck2str tokenflag2str_table[] = {
	CK2STR_ENTRY(CKF_RNG),
	CK2STR_ENTRY(CKF_WRITE_PROTECTED),
	CK2STR_ENTRY(CKF_LOGIN_REQUIRED),
	CK2STR_ENTRY(CKF_USER_PIN_INITIALIZED),
	CK2STR_ENTRY(CKF_RESTORE_KEY_NOT_NEEDED),
	CK2STR_ENTRY(CKF_CLOCK_ON_TOKEN),
	CK2STR_ENTRY(CKF_PROTECTED_AUTHENTICATION_PATH),
	CK2STR_ENTRY(CKF_DUAL_CRYPTO_OPERATIONS),
	CK2STR_ENTRY(CKF_TOKEN_INITIALIZED),
	CK2STR_ENTRY(CKF_SECONDARY_AUTHENTICATION),
	CK2STR_ENTRY(CKF_USER_PIN_COUNT_LOW),
	CK2STR_ENTRY(CKF_USER_PIN_FINAL_TRY),
	CK2STR_ENTRY(CKF_USER_PIN_LOCKED),
	CK2STR_ENTRY(CKF_USER_PIN_TO_BE_CHANGED),
	CK2STR_ENTRY(CKF_SO_PIN_COUNT_LOW),
	CK2STR_ENTRY(CKF_SO_PIN_FINAL_TRY),
	CK2STR_ENTRY(CKF_SO_PIN_LOCKED),
	CK2STR_ENTRY(CKF_SO_PIN_TO_BE_CHANGED),
	CK2STR_ENTRY(CKF_ERROR_STATE),
};

static const char *token_flags2str(CK_ULONG flags)
{
	const int count = sizeof(tokenflag2str_table) / sizeof(struct ck2str);
	int n;

	for (n = 0; n < count; n++) {
		if (flags & tokenflag2str_table[n].id)
			return tokenflag2str_table[n].string;
	}

	return NULL;
}

static struct ck2str mechaflag2str_table[] = {
	CK2STR_ENTRY(CKF_HW),
	CK2STR_ENTRY(CKF_ENCRYPT),
	CK2STR_ENTRY(CKF_DECRYPT),
	CK2STR_ENTRY(CKF_DIGEST),
	CK2STR_ENTRY(CKF_SIGN),
	CK2STR_ENTRY(CKF_SIGN_RECOVER),
	CK2STR_ENTRY(CKF_VERIFY),
	CK2STR_ENTRY(CKF_VERIFY_RECOVER),
	CK2STR_ENTRY(CKF_GENERATE),
	CK2STR_ENTRY(CKF_GENERATE_KEY_PAIR),
	CK2STR_ENTRY(CKF_WRAP),
	CK2STR_ENTRY(CKF_UNWRAP),
	CK2STR_ENTRY(CKF_DERIVE),
	CK2STR_ENTRY(CKF_EC_F_P),
	CK2STR_ENTRY(CKF_EC_F_2M),
	CK2STR_ENTRY(CKF_EC_ECPARAMETERS),
	CK2STR_ENTRY(CKF_EC_NAMEDCURVE),
	CK2STR_ENTRY(CKF_EC_UNCOMPRESS),
	CK2STR_ENTRY(CKF_EC_COMPRESS),
	CK2STR_ENTRY(CKF_EXTENSION),
};

static const char *mecha_flags2str(CK_ULONG flags)
{
	const int count = sizeof(mechaflag2str_table) / sizeof(struct ck2str);
	int n;

	for (n = 0; n < count; n++) {
		if (flags & mechaflag2str_table[n].id)
			return mechaflag2str_table[n].string;
	}

	return NULL;
}

enum ck_debug_flag_type {
	CKDBG_SLOT,
	CKDBG_TOKEN,
	CKDBG_KEY,
	CKDBG_CERTIF,
	CKDBG_MECHA,
};

static char *__flag2str(CK_ULONG flags, enum ck_debug_flag_type type)
{
	char *str = NULL;
	size_t size = 0;
	int mask = 1;

	for (mask = 1; flags && mask; flags &= ~mask, mask = mask << 1) {
		char const *label = NULL;
		size_t label_size;
		char *nstr;

		if (!(flags & mask))
			continue;

		switch(type) {
		case CKDBG_SLOT:
			label = slot_flags2str(mask);
			break;
		case CKDBG_TOKEN:
			label = token_flags2str(mask);
			break;
		case CKDBG_MECHA:
			label = mecha_flags2str(mask);
			break;
		default:
			return NULL;
		}

		if (!label)
			continue;

		/* 4 digit prefix "CKF_" is not dumped */
		if (!memcmp(label, "CKF_", 4))
			label += 4;

		/* Extra space digit */
		label_size = strlen(label) + 1;

		/* Always allocate 1 more digit for terminal '\0' */
		nstr = TEE_Realloc(str, size + label_size + 1);
		if (!nstr) {
			TEE_Free(str);
			return NULL;
		}

		snprintf(nstr + size, label_size + 1, "%s ", label);
		str = nstr;
		size += label_size;
	}

	return str;
}

char *ck_slot_flag2str(CK_ULONG flags)
{
	return __flag2str(flags, CKDBG_SLOT);
}

char *ck_token_flag2str(CK_ULONG flags)
{
	return __flag2str(flags, CKDBG_TOKEN);
}

char *ck_mecha_flag2str(CK_ULONG flags)
{
	return __flag2str(flags, CKDBG_MECHA);
}

static struct ck2str class2str_table[] = {
	CK2STR_ENTRY(CKO_DATA),
	CK2STR_ENTRY(CKO_CERTIFICATE),
	CK2STR_ENTRY(CKO_PUBLIC_KEY),
	CK2STR_ENTRY(CKO_PRIVATE_KEY),
	CK2STR_ENTRY(CKO_SECRET_KEY),
	CK2STR_ENTRY(CKO_HW_FEATURE),
	CK2STR_ENTRY(CKO_DOMAIN_PARAMETERS),
	CK2STR_ENTRY(CKO_MECHANISM),
	CK2STR_ENTRY(CKO_OTP_KEY),
	CK2STR_ENTRY(CKO_VENDOR_DEFINED),
};

const char *ckclass2str(CK_ULONG id)
{
	const int count = sizeof(class2str_table) / sizeof(struct ck2str);
	int n;

	for (n = 0; n < count; n++)
		if (id == class2str_table[n].id)
			return class2str_table[n].string;

	return NULL;
}

static struct ck2str symkey2str_table[] = {
	CK2STR_ENTRY(CKK_RSA),
	CK2STR_ENTRY(CKK_DSA),
	CK2STR_ENTRY(CKK_DH),
	CK2STR_ENTRY(CKK_ECDSA),
	CK2STR_ENTRY(CKK_EC),
	CK2STR_ENTRY(CKK_X9_42_DH),
	CK2STR_ENTRY(CKK_KEA),
	CK2STR_ENTRY(CKK_GENERIC_SECRET),
	CK2STR_ENTRY(CKK_RC2),
	CK2STR_ENTRY(CKK_RC4),
	CK2STR_ENTRY(CKK_DES),
	CK2STR_ENTRY(CKK_DES2),
	CK2STR_ENTRY(CKK_DES3),
	CK2STR_ENTRY(CKK_CAST),
	CK2STR_ENTRY(CKK_CAST3),
	CK2STR_ENTRY(CKK_CAST5),
	CK2STR_ENTRY(CKK_CAST128),
	CK2STR_ENTRY(CKK_RC5),
	CK2STR_ENTRY(CKK_IDEA),
	CK2STR_ENTRY(CKK_SKIPJACK),
	CK2STR_ENTRY(CKK_BATON),
	CK2STR_ENTRY(CKK_JUNIPER),
	CK2STR_ENTRY(CKK_CDMF),
	CK2STR_ENTRY(CKK_AES),
	CK2STR_ENTRY(CKK_BLOWFISH),
	CK2STR_ENTRY(CKK_TWOFISH),
	CK2STR_ENTRY(CKK_SECURID),
	CK2STR_ENTRY(CKK_HOTP),
	CK2STR_ENTRY(CKK_ACTI),
	CK2STR_ENTRY(CKK_CAMELLIA),
	CK2STR_ENTRY(CKK_ARIA),
	CK2STR_ENTRY(CKK_MD5_HMAC),
	CK2STR_ENTRY(CKK_SHA_1_HMAC),
	CK2STR_ENTRY(CKK_RIPEMD128_HMAC),
	CK2STR_ENTRY(CKK_RIPEMD160_HMAC),
	CK2STR_ENTRY(CKK_SHA256_HMAC),
	CK2STR_ENTRY(CKK_SHA384_HMAC),
	CK2STR_ENTRY(CKK_SHA512_HMAC),
	CK2STR_ENTRY(CKK_SHA224_HMAC),
	CK2STR_ENTRY(CKK_SEED),
	CK2STR_ENTRY(CKK_GOSTR3410),
	CK2STR_ENTRY(CKK_GOSTR3411),
	CK2STR_ENTRY(CKK_GOST28147),
	CK2STR_ENTRY(CKK_VENDOR_DEFINED),
};

static struct ck2str certif2str_table[] = {
	CK2STR_ENTRY(CK_CERTIFICATE_CATEGORY_UNSPECIFIED),
	CK2STR_ENTRY(CK_CERTIFICATE_CATEGORY_TOKEN_USER),
	CK2STR_ENTRY(CK_CERTIFICATE_CATEGORY_AUTHORITY),
	CK2STR_ENTRY(CK_CERTIFICATE_CATEGORY_OTHER_ENTITY),
};

static struct ck2str domain2str_table[] = {
	CK2STR_ENTRY(CK_SECURITY_DOMAIN_UNSPECIFIED),
	CK2STR_ENTRY(CK_SECURITY_DOMAIN_MANUFACTURER),
	CK2STR_ENTRY(CK_SECURITY_DOMAIN_OPERATOR),
	CK2STR_ENTRY(CK_SECURITY_DOMAIN_THIRD_PARTY),
};

const char *cktype2str(CK_ULONG id, CK_ULONG class)
{
	int count;
	struct ck2str *table;
	int n;

	switch (class) {
	case CKO_DATA:
		return NULL;
	case CKO_CERTIFICATE:
		count = sizeof(certif2str_table);
		table = certif2str_table;
		break;
	case CKO_PUBLIC_KEY:
		return NULL;
	case CKO_PRIVATE_KEY:
		return NULL;
	case CKO_SECRET_KEY:
		count = sizeof(symkey2str_table);
		table = symkey2str_table;
		break;
	case CKO_HW_FEATURE:
		return NULL;
	case CKO_DOMAIN_PARAMETERS:
		count = sizeof(domain2str_table);
		table = domain2str_table;
		break;
	case CKO_MECHANISM:
		return NULL;
	case CKO_OTP_KEY:
		return NULL;
	default:
		return NULL;
	}

	count /= sizeof(struct ck2str);
	for (n = 0; n < count; n++)
		if (id == table[n].id)
			return table[n].string;

	return NULL;
}
