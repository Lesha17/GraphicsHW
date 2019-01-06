__kernel void test(__global uchar * message, int width)
{
    // получаем текущий id.
    int x = get_global_id(0);
    int y = get_global_id(1);
    
    message[4 * (y * width + x)] = x;
    message[4 * (y * width + x) + 1] = x + y;
    message[4 * (y * width + x) + 2] = y;
    message[4 * (y * width + x) + 3] = 255;
}
