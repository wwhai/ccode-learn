#include <stdio.h>
#include <modbus.h>

int main(void)
{
    modbus_t *ctx;
    uint16_t tab_reg[64];
    int rc;

    ctx = modbus_new_tcp("192.168.1.10", 502); // IP地址和端口号
    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_slave(ctx, 1); // 从站设备地址

    rc = modbus_connect(ctx);
    if (rc == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // 读取寄存器
    rc = modbus_read_registers(ctx, 0, 10, tab_reg); // 起始地址和寄存器数量
    if (rc == -1)
    {
        fprintf(stderr, "Failed to read registers: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // 处理数据
    printf("Register values:");
    for (int i = 0; i < rc; i++)
    {
        printf(" %d", tab_reg[i]);
    }
    printf("\n");

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}