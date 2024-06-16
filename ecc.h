#ifndef   ECC_H_INCLUDED
#define   ECC_H_INCLUDED

#include "stdmoh.h"


typedef struct ecc_point_t
{
    int64_t x, y;
    bool_t is_identity;
} ecc_point_t;

typedef struct ecc_curve_t
{
    int64_t a, b, p;
} ecc_curve_t;

typedef struct ecc_cypher_pair_t
{
    ecc_point_t kG, Pm_knG;
} ecc_cypher_pair_t;
typedef struct ecc_cypher_t
{
    ecc_cypher_pair_t *pairs;
    int64_t           size;
} ecc_cypher_t;

typedef struct ecc_pub_key_t
{
    ecc_point_t G, nG;
} ecc_pub_key_t;
typedef struct ecc_priv_key_t
{
    int64_t     n;
    ecc_point_t G;
} ecc_priv_key_t;


//* Functions
//* Modulo Arithmetic
int64_t modinv(int64_t a, int64_t m);

//* Elliptic Curve
void ecc_point_init(ecc_point_t *P, int64_t x, int64_t y, bool_t is_identity);
void ecc_point_copy(ecc_point_t *P, ecc_point_t *Q);

void ecc_add(ecc_point_t *P, ecc_point_t *Q, ecc_point_t *R, int64_t a, int64_t p);
void ecc_scal_mul(ecc_point_t *G, ecc_point_t *nG, int64_t n, int64_t a, int64_t p);
int64_t ecc_rand();
bool_t ecc_is_valid_x_point(int64_t y, int64_t p);
void ecc_str_ec_encoding(ecc_point_t *P, ecc_curve_t *curve, int16_t m);
void ecc_encrypt(ecc_pub_key_t *key, ecc_curve_t *curve, str_t m, ecc_cypher_t *c);
void ecc_decrypt(ecc_priv_key_t *key, ecc_curve_t *curve, ecc_cypher_t *c, str_t d);



#endif // ECC_H_INCLUDED
