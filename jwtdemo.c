#include "jwt.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
// gcc jwtdemo.c -ljwt -o jwtdemo
int main()
{
    const char *json = "{\"id\":\"12345\","
                       "\"iss\":\"localhost\"}";

    // jwt 指针
    jwt_t *jwt = NULL;

    // 密钥
    const char key[] = "My Passphrase";

    // 生成jwt对象
    int ret = jwt_new(&jwt);
    assert(ret == 0);
    assert(jwt != NULL);
    printf("空的jwt对象\n%s\n", jwt_dump_str(jwt, 1));

    // 添加字符串内容
    ret = jwt_add_grant(jwt, "iss", "files.cyphre.com");
    assert(ret == 0);
    printf("添加字符串内容\n%s\n", jwt_dump_str(jwt, 1));

    // 添加整数内容
    ret = jwt_add_grant_int(jwt, "iat", (long)time(NULL));
    assert(ret == 0);
    printf("添加整数内容\n%s\n", jwt_dump_str(jwt, 1));

    // 添加json内容
    // 上述iat，iss会被添加到json中
    // 上述iss会被json中的iss覆盖
    ret = jwt_add_grants_json(jwt, json);
    assert(ret == 0);
    printf("添加json内容\n%s\n", jwt_dump_str(jwt, 1));

    // 设置算法和密钥
    // 会自动生成header部分
    ret = jwt_set_alg(jwt, JWT_ALG_HS256, (unsigned char *)key, strlen(key));
    assert(ret == 0);
    printf("设置算法和密钥\n%s\n", jwt_dump_str(jwt, 1));

    // 输出最终的jwt token
    char *jwt_str = jwt_encode_str(jwt);
    printf("输出最终的jwt\n%s\n", jwt_str);

    // 用来存放decode后的jwt对象
    jwt_t *jwt2 = NULL;

    // decode
    ret = jwt_decode(&jwt2, jwt_str, (const char unsigned *)key, strlen(key));
    assert(ret == 0);

    // decode后的json内容
    printf("\nDecode后的json内容\n%s\n", jwt_dump_str(jwt2, 1));

    jwt_free(jwt);
    jwt_free(jwt2);

    return 0;
}