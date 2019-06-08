#include <stdio.h>
#include <string.h>
#include <stdint-gcc.h>

/** Symbol table to transform nibbles into 6 bits, index stands for nibble
    value. Values are taken from the table in the standard. */
const uint8_t gprc_3OutOf6Symbols[] = {
  22U, /* 0x0: 0b010110 */
  13U, /* 0x1: 0b001101 */
  14U, /* 0x2: 0b001110 */
  11U, /* 0x3: 0b001011 */
  28U, /* 0x4: 0b011100 */
  25U, /* 0x5: 0b011001 */
  26U, /* 0x6: 0b011010 */
  19U, /* 0x7: 0b010011 */
  44U, /* 0x8: 0b101100 */
  37U, /* 0x9: 0b100101 */
  38U, /* 0xA: 0b100110 */
  35U, /* 0xB: 0b100011 */
  52U, /* 0xC: 0b110100 */
  49U, /* 0xD: 0b110001 */
  50U, /* 0xE: 0b110010 */
  41U  /* 0xF: 0b101001 */
};

/** Reverse symbol table to transform 6 bits into nibbles. Only fields defined
    in grc_phy_3OutOf6Symbols[] are valid. If Flash size is more critical than
    speed, the array can be removed and the grc_phy_3OutOf6Symbols[] can be
    parsed.

    Valid combinations are:
    010110 -> 0x0 -> 22      001101 -> 0x1 -> 13
    001110 -> 0x2 -> 14      001011 -> 0x3 -> 11
    011100 -> 0x4 -> 28      011001 -> 0x5 -> 25
    011010 -> 0x6 -> 26      010011 -> 0x7 -> 19
    101100 -> 0x8 -> 44      100101 -> 0x9 -> 37
    100110 -> 0xA -> 38      100011 -> 0xB -> 35
    110100 -> 0xC -> 52      110001 -> 0xD -> 49
    110010 -> 0xE -> 50      101001 -> 0xF -> 41

    All other combinations are invalid. */
#define SF_3OO6_ERR4 0xFFU
const uint8_t gprc_3OutOf6ReverseSymbols[] = {
  /* 13 cases still invalid (no guess), each symbol represented 3 to 4 times */
  SF_3OO6_ERR4, SF_3OO6_ERR4, SF_3OO6_ERR4, 0x07U,    SF_3OO6_ERR4, 0x01U,    0x02U,    0x02U,
  SF_3OO6_ERR4, 0x03U,    0x03U,    0x03U,    0x01U,    0x01U,    0x02U,    0x02U,
  0x05U,    0x05U,    0x07U,    0x07U,    0x00U,    0x00U,    0x00U,    0x00U,
  0x04U,    0x05U,    0x06U,    0x06U,    0x04U,    0x04U,    0x06U,    SF_3OO6_ERR4,
  SF_3OO6_ERR4, 0x0BU,    0x0BU,    0x0BU,    0x09U,    0x09U,    0x0AU,    0x09U,
  0x08U,    0x0FU,    0x0FU,    0x0FU,    0x08U,    0x08U,    0x0AU,    SF_3OO6_ERR4,
  0x0DU,    0x0DU,    0x0EU,    0x0EU,    0x0CU,    0x0CU,    0x0AU,    SF_3OO6_ERR4,
  0x0DU,    0x0DU,    0x0EU,    SF_3OO6_ERR4, 0x0CU,    SF_3OO6_ERR4, SF_3OO6_ERR4, SF_3OO6_ERR4
};

uint16_t sf_3oo6_decode(uint8_t *pc_dst, uint8_t *pc_src, uint16_t i_len)
{
  /* Counter variable. */
  uint16_t i = 0U;
  /* Return value. */
  uint16_t i_retLen = 0U;
  /* Memory to save 2 decoded bytes into. */
  union
  {
    uint32_t u32;
    uint8_t  u8[3U];
  }  l_tmpSymbol;
  /* Restore default value. */
  l_tmpSymbol.u32 = 0U;

  for(i = 0U; i < i_len; i += 3)
  {
    /* Transform 3 received bytes into 2 data bytes */
    /* Fetch the value */
    l_tmpSymbol.u8[2U] = pc_src[i + 0U];
    l_tmpSymbol.u8[1U] = pc_src[i + 1U];
    l_tmpSymbol.u8[0U] = pc_src[i + 2U];

    /* Extract 6 bits and look for matching nibble in the table */
    (*pc_dst) = gprc_3OutOf6ReverseSymbols[l_tmpSymbol.u8[2U] >> 2U] << 4U;
    #if BIG_ENDIAN
    /* swap the byte order */
    ENDIAN_SWAP_32(l_tmpSymbol.u32);
    #endif

    l_tmpSymbol.u32 <<= 6U;
    #if BIG_ENDIAN
    /* swap the byte order */
    ENDIAN_SWAP_32(l_tmpSymbol.u32);
    #endif

    (*pc_dst) |= gprc_3OutOf6ReverseSymbols[l_tmpSymbol.u8[2U] >> 2U];
    #if BIG_ENDIAN
    /* swap the byte order */
    ENDIAN_SWAP_32(l_tmpSymbol.u32);
    #endif

    l_tmpSymbol.u32 <<= 6U;
    #if BIG_ENDIAN
    /* swap the byte order */
    ENDIAN_SWAP_32(l_tmpSymbol.u32);
    #endif

    pc_dst++;

    /* Extract 6 bits and look for matching nibble in the table */
    (*pc_dst) = gprc_3OutOf6ReverseSymbols[l_tmpSymbol.u8[2U] >> 2U] << 4U;
    #if BIG_ENDIAN
    /* swap the byte order */
    ENDIAN_SWAP_32(l_tmpSymbol.u32);
    #endif

    l_tmpSymbol.u32 <<= 6U;
    #if BIG_ENDIAN
    /* swap the byte order */
    ENDIAN_SWAP_32(l_tmpSymbol.u32);
    #endif

    (*pc_dst) |= gprc_3OutOf6ReverseSymbols[l_tmpSymbol.u8[2U] >> 2U];
    #if BIG_ENDIAN
    /* swap the byte order */
    ENDIAN_SWAP_32(l_tmpSymbol.u32);
    #endif

    l_tmpSymbol.u32 <<= 6U;
    #if BIG_ENDIAN
    /* swap the byte order */
    ENDIAN_SWAP_32(l_tmpSymbol.u32);
    #endif

    pc_dst++;

    i_retLen += 2U;
  } /* for */
  return i_retLen;
}

static
uint16_t sf_3oo6_getEncodedLen(uint16_t i_len)
{
  uint16_t i_dataLen = 0U;

  /* Calculates the number of bytes after encoding. */
  i_dataLen = ((((i_len) * 3U) + 1U) / 2U);

  return i_dataLen;
}

#define SF_3OO6_BITS_PER_NIBBLE                              (6U)
#define SF_3OO6_BYTES_PER_STEP                               (2U)
#define SF_3OO6_STEP_SIZE                                    (3U)

static
uint16_t sf_3oo6_encode(uint8_t *pc_dst, uint8_t *pc_src, uint16_t i_len)
{
  /* Counter variables. */
  uint16_t i = 0U, j = 0U;
  /* Return value. */
  uint16_t i_retLen = 0U;
  /* Memory to save 2 encoded bytes into. */
  union
  {
    uint32_t u32;
    uint8_t  u8[3U];
  } l_tmpSymbol;
  /* Restore default value. */
  l_tmpSymbol.u32 = 0U;

  /* Step through the outgoing data stream */
  for(i = 0U; i < i_len; i += SF_3OO6_BYTES_PER_STEP)
  {
    /* Byte order is 2 - 1 - 0 so the 6 encoded bits have to be moved from
       low to up. */
    l_tmpSymbol.u32 = (uint32_t) gprc_3OutOf6Symbols[((*pc_src) & 0xF0U) >> 4U];
    l_tmpSymbol.u32 <<= SF_3OO6_BITS_PER_NIBBLE;
    l_tmpSymbol.u32 += (uint32_t) gprc_3OutOf6Symbols[((*pc_src) & 0x0FU)];
    l_tmpSymbol.u32 <<= SF_3OO6_BITS_PER_NIBBLE;
    pc_src++;
    l_tmpSymbol.u32 += (uint32_t) gprc_3OutOf6Symbols[((*pc_src) & 0xF0U) >> 4U];
    l_tmpSymbol.u32 <<= SF_3OO6_BITS_PER_NIBBLE;
    l_tmpSymbol.u32 += (uint32_t) gprc_3OutOf6Symbols[((*pc_src) & 0x0FU)];
    pc_src++;

    #if BIG_ENDIAN
    /* swap the byte order */
    ENDIAN_SWAP_32(l_tmpSymbol.u32);
    #endif

    /* 24 bits extracted --> store them into the output buffer
       Attention: Order is 2-1-0 */
    for(j = 0U;j < SF_3OO6_STEP_SIZE ;j++)
    {
      (*pc_dst) = l_tmpSymbol.u8[2U - j];
      pc_dst++;
    } /* for */
  } /* for */

  /* Calcualtes the number of encoded bytes. */
  i_retLen = sf_3oo6_getEncodedLen(i_len);

  if((i_len % 2U) != 0U)
  {
    /* Delete the last nibble and enter 0b0101 */
    pc_dst--;
    *pc_dst &= 0xF0U;
    /* Set the nibble depending on the last bit of the crc.
       According to the EN13757-4 2013-11 page 20 Note c (T-mode transmitter)*/
    if(*pc_dst &= 0x10U)
      *pc_dst |= 0x05U;
    else
      *pc_dst |= 0x0AU;
  } /* if */

  return i_retLen;
}

void main (void)
{
    uint8_t decoded [] = {0x0F, 0x44, 0xAE, 0x0C,
                          0x78, 0x56, 0x34, 0x12,
                          0x01, 0x07, 0x44, 0x47,
                          0x78, 0x0B, 0x13, 0x43,
                          0x65, 0x87, 0x1E, 0x6D};
    uint8_t encoded [sf_3oo6_getEncodedLen(sizeof(decoded))];
    memset(encoded, 0, sizeof(encoded));
    sf_3oo6_encode(encoded, decoded, sizeof(encoded));
  
    memset(decoded, 0 , sizeof(decoded));
    sf_3oo6_decode(decoded, encoded, sizeof(encoded));


}