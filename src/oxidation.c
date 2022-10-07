#include "oxidation.h"

bool   is_err(Result res) { return res.err != NULL; }
bool   is_ok(Result res) { return res.err == NULL; }
Result Ok(void* val) { return (Result){val, NULL}; }
Result Err(char* err) { return (Result){NULL, err}; }
