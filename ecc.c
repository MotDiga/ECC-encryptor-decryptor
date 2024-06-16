#include "ecc.h"


//* Modulo Arithmatic
int64_t modinv(int64_t a, int64_t m)
{
    int64_t x = 1LL, y = 0LL, q, t, m0 = m;
    a = ((a%m) + m) % m;

    while (a > 1LL)
    {
        q = a/m, t = m;

        m = a % m, a = t, t = y;

        y = x - q*y;
        x = t;
    }

    return ((x%m0) + m0) % m0;
}


//* Elliptic Curves
void ecc_point_init(ecc_point_t *P, int64_t x, int64_t y, bool_t is_identity)
{
    P->x = x, P->y = y; P->is_identity = is_identity;
}
// Copy Q in P
void ecc_point_copy(ecc_point_t *P, ecc_point_t *Q)
{
    P->x = Q->x, P->y = Q->y; P->is_identity = Q->is_identity;
}

void ecc_add(ecc_point_t *P, ecc_point_t *Q, ecc_point_t *R, int64_t a, int64_t p)
{
    if (P->is_identity == true)
    {
        R->x = Q->x;
        R->y = Q->y;
        R->is_identity = Q->is_identity;
        return;
    }
    else if (Q->is_identity == true)
    {
        R->x = P->x;
        R->y = P->y;
        R->is_identity = P->is_identity;
        return;
    }
    else
    {
        int64_t delta;
        if ((P->x == Q->x) && (P->y == Q->y))
        {
            if (P->y == 0LL)
            {
                R->x = 0LL;
                R->y = 0LL;
                R->is_identity = true;
                return;
            }
            delta = ((3LL*(P->x*P->x%p) + a) * modinv(2LL*P->y, p)) % p;
        }
        else
        {
            if (P->x == Q->x)
            {
                R->x = 0LL;
                R->y = 0LL;
                R->is_identity = true;
                return;
            }
            delta = ((P->y - Q->y) * modinv(P->x - Q->x, p)) % p;
        }

        delta = (delta + p) % p;
        R->x = ((delta*delta%p) - P->x - Q->x) % p;
        R->x = (R->x + p) % p;
        R->y = (((delta*(P->x - R->x))%p) - P->y) % p;
        R->y = (R->y + p) % p;
        R->is_identity = false;
    }
}
void ecc_scal_mul(ecc_point_t *G, ecc_point_t *nG, int64_t n, int64_t a, int64_t p)
{
    ecc_point_t Tmp1 = {G->x, G->y, false}, Tmp2;
    ecc_point_init(nG, 0LL, 0LL, true);
    int64_t tmpx, delta;
    while (n)
    {
        ecc_point_copy(&Tmp2, nG);
        if (n & 0x0000000000000001)
        {
            ecc_add(&Tmp1, &Tmp2, nG, a, p);
        }
        n >>= 1;

        // Doubling Tmp1
        if (Tmp1.y == 0LL)
        {
            return;
        }

        delta = ((3LL*(Tmp1.x*Tmp1.x%p) + a) * modinv(2LL*Tmp1.y, p)) % p;
        delta = (delta + p) % p;
        tmpx = Tmp1.x;
        Tmp1.x = ((delta*delta%p) - 2LL*Tmp1.x) % p;
        Tmp1.x = (Tmp1.x + p) % p;
        Tmp1.y = (((delta*(tmpx - Tmp1.x))%p) - Tmp1.y) % p;
        Tmp1.y = (Tmp1.y + p) % p;
    }
}

int64_t ecc_rand()
{
    return rand();
    //TODO: ecc rand
}

bool_t ecc_is_valid_x_point(int64_t y, int64_t p)
{
    int64_t n = (p - 1LL)/2LL, t = y, quad_resid = 1LL;
    while (n)
    {
        if (n & 0x0000000000000001)
        {
            quad_resid = (quad_resid*t) % p;
        }
        n >>= 1;

        t = (t*t) % p;
    }

    if (quad_resid == 1LL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ecc_str_ec_encoding(ecc_point_t *P, ecc_curve_t *curve, int16_t m)
{
    P->is_identity = false;
    P->x = ((int64_t)m)*256LL;
    P->y = (((P->x*P->x)%curve->p)*P->x + ((P->x*curve->a)%curve->p) + curve->b) % curve->p;
    while (!ecc_is_valid_x_point(P->y, curve->p))
    {
        P->x++;
        P->y = (((P->x*P->x)%curve->p)*P->x + ((P->x*curve->a)%curve->p) + curve->b) % curve->p;
    }
    // the P->y = sqrt(current P->y)
    // if (p % 4 == 3)
    int64_t n = (curve->p + 1LL)/4LL, t = P->y;
    P->y = 1LL;
    while (n)
    {
        if (n & 0x0000000000000001)
        {
            P->y = (P->y*t) % curve->p;
        }
        n >>= 1;

        t = (t*t) % curve->p;
    }
    // else: TODO:
}

void ecc_encrypt(ecc_pub_key_t *key, ecc_curve_t *curve, str_t m, ecc_cypher_t *c)
{
    int64_t i, k;
    ecc_point_t Pm, knG;
    for (i = 0LL; i < c->size; i++)
    {
        ecc_str_ec_encoding(&Pm, curve, ((int16_t*)m)[i]);
        k = ecc_rand();
        ecc_scal_mul(&(key->G), &((c->pairs[i]).kG), k, curve->a, curve->p);
        ecc_scal_mul(&(key->nG), &knG, k, curve->a, curve->p);
        ecc_add(&Pm, &knG, &((c->pairs[i]).Pm_knG), curve->a, curve->p);
    }
}

void ecc_decrypt(ecc_priv_key_t *key, ecc_curve_t *curve, ecc_cypher_t *c, str_t d)
{
    int64_t i;
    str16_t d_itr = d;
    ecc_point_t inv_nkG, Pm;
    for (i = 0LL; i < c->size; i++)
    {
        ecc_scal_mul(&((c->pairs[i]).kG), &inv_nkG, key->n, curve->a, curve->p);
        inv_nkG.y = (-inv_nkG.y + curve->p) % curve->p;
        ecc_add(&((c->pairs[i]).Pm_knG), &inv_nkG, &Pm, curve->a, curve->p);
        d_itr[i] = Pm.x / 256LL;
    }
    d[2LL*i - ((d_itr[i-1] < 256) ? 1LL : 0LL)] = '\0';
}
