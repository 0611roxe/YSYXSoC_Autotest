#define SPI_BASE 0x10001000L
#define SPI_SS  0x18
#define SPI_CTRL 0x10
#define SPI_DIVIDER 0x14
#define SPI_RX0 0x0
#define SPI_TX0 0x0

unsigned int flash_read(unsigned int tx) {
    *(volatile unsigned int *)(SPI_BASE + SPI_TX0 + 4) = tx; //设置指令到Tx1等待传输给falsh,读地址0,指令是03h
    *(volatile char *)(SPI_BASE + SPI_SS) = 0b00000001; //设置ss为falsh
    *(volatile unsigned int *)(SPI_BASE + SPI_DIVIDER) = 0x00000004;//除数寄存器
    *(volatile unsigned int *)(SPI_CTRL + SPI_BASE) = 0x00000040;//传输大小,64bit
    *(volatile unsigned int *)(SPI_CTRL + SPI_BASE) = 0x00000140; //开始传输信号,使GOBSY为1
    while((*(volatile unsigned int *)(SPI_CTRL+SPI_BASE) & 0x00000100) == 0x00000100); //等待GOBSY为0
    unsigned int flash_data = *(volatile unsigned int*)(SPI_BASE + SPI_TX0);
    return flash_data;
}

int main() {
    unsigned int flash_data = flash_read(0x03000000);
    if (flash_data == 0x13040000) {
        ;
    }
    return 0;
}
