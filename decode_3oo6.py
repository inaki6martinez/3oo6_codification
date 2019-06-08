# Codified : Value
table_3oo6 = {
  22 : 0x0,
  13 : 0x1,
  14 : 0x2,
  11 : 0x3,
  28 : 0x4,
  25 : 0x5,
  26 : 0x6,
  19 : 0x7,
  44 : 0x8,
  37 : 0x9,
  38 : 0xA,
  35 : 0xB,
  52 : 0xC,
  49 : 0xD,
  50 : 0xE,
  41 : 0xF,
}

def find_codified_in_table(codified):
  for c, v in table_3oo6.items():
    if c == codified:
      return v
  assert False

def find_value_in_table(value):
  for c, v in table_3oo6.items():
    if v == value:
      return c
  assert False


value = [0x5A, 0x97, 0x1C, 0x9B, 0x25, 0xB4, 0x4E, 0xC6, 0x5A, 0x2D, 0xC3, 0x4E, 0x58, 0xD5, 0x93, 0x71,
         0xC7, 0x13, 0x4E, 0xC5, 0xA3, 0x34, 0xB7, 0x0B, 0x69, 0x9B, 0x13, 0x37, 0x26, 0xB1]

value_32 = []
for byte in range(0,len(value),3):
  if byte+2 < len(value):
    value_32.append(value[byte] << 16 | value[byte+1] << 8 | value[byte+2])
  elif byte+1 < len(value):
    value_32.append(value[byte] << 8 | value[byte+1])
  else:
    value_32.append(value[byte])

decoded = []
offset = 0;
for group in value_32:
  decoded.append(0)
  decoded[offset] |= find_codified_in_table(group >> 18) << 4
  decoded[offset] |= find_codified_in_table((group >> 12) & 0x3F)
  offset += 1
  decoded.append(0)
  decoded[offset] |= find_codified_in_table((group >> 6) & 0x3F) << 4
  decoded[offset] |= find_codified_in_table(group & 0x3F)
  offset += 1

result =[]
for by in decoded:
  result.append(f'{by:02x}')
print (*result)
