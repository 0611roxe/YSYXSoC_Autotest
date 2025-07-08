#define SPI_BASE 0x10001000L
#define SPI_SS  0x18
#define SPI_CTRL 0x10
#define SPI_DIVIDER 0x14
#define SPI_RX0 0x0
#define SPI_TX0 0x0
int main() {
    *(volatile char *)(SPI_BASE + SPI_SS) = 0b10000000; //设置ss
    *(volatile unsigned int *)(SPI_BASE + SPI_DIVIDER) = 0x00000004;
    *(volatile unsigned int *)(SPI_CTRL + SPI_BASE) = 0x00000010;//传输大小,16bit
    *(volatile  int *)(SPI_BASE + SPI_TX0) = 0x0F00;
    *(volatile unsigned int *)(SPI_CTRL + SPI_BASE) = 0x00000110; //开始传输信号,使GOBSY为1
    while((*(volatile unsigned int *)(SPI_CTRL+SPI_BASE) & 0x00000100) == 0x00000100); //等待GOBSY为0

    return 0;
}
