#include "ecc.h"



int32_t main(int32_t argc, str_t argv[])
{
    srand(time(0));

    //* Coosing a Curve
    ecc_curve_t curve = {-4559LL, 536283647LL, 1070531951LL};

    //* The message to be encrypted
    //str_t m = "Salam Alicom ou Aid Said";
    char m[1024];
    printf("Enter the message to be encrypted: ");
    scanf("%[^\n]", m);
    putchar('\n');
    //* The cypher buffer
    int64_t size = (strlen(m) + 1LL) / 2LL;
    ecc_cypher_pair_t c_pair[size];
    ecc_cypher_t c = {c_pair, size};

    //* The priv/pub keys
    ecc_priv_key_t priv_key;
    priv_key.n = 847152311;
    ecc_point_init(&(priv_key.G), 938814828LL, 619572498LL, false);
    ecc_pub_key_t pub_key;
    ecc_point_init(&(pub_key.G), 938814828LL, 619572498LL, false);
    ecc_scal_mul(&(pub_key.G), &(pub_key.nG), priv_key.n, curve.a, curve.p);

    ecc_encrypt(&pub_key, &curve, m, &c);
    printf("The encrypted message:\n");
    for (int64_t i = 0LL; i < c.size; i++) printf("((%10lld, %10lld)[%s], (%10lld, %10lld)[%s])\n", c_pair[i].kG.x, c_pair[i].kG.y, c_pair[i].kG.is_identity ? "identity" : "element", c_pair[i].Pm_knG.x, c_pair[i].Pm_knG.y, c_pair[i].kG.is_identity ? "identity" : "element");
    putchar('\n');

    char d[1024];
    ecc_decrypt(&priv_key, &curve, &c, d);
    printf("The decrypted message: \"%s\"\n", d);

    return 0;
}
