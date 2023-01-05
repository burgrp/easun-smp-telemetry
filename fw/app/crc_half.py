def crc_half(data):

    crc_ta = (0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef)

    crc = 0
    for byte in data:
        da = crc >> 12 & 0x0F
        crc <<= 4
        crc ^= crc_ta[da ^ (byte >> 4)]

        da = crc >> 12 & 0x0F
        crc <<= 4
        crc ^= crc_ta[da ^ (byte & 0x0f)]

    crcLow = crc & 0xFF
    crcHigh = (crc >> 8) & 0xFF

    if crcLow == 0x28 or crcLow == 0x0d or crcLow == 0x0a:
        crcLow+=1

    if crcHigh == 0x28 or crcHigh == 0x0d or crcHigh == 0x0a:
        crcHigh+=1

    crc = (crcHigh << 8) | crcLow

    return (crcHigh, crcLow)

# B7 A9 ?
# crcHigh, crcLow = crc_half(b'QPIGS')
# print(hex(crcHigh), hex(crcLow))