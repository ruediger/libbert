/* Test the C API */

#include <bert.h>

#include <stdio.h>
#include <string.h>

typedef bert_byte_t byte_t;

void test(byte_t *in, size_t len) {
  bert_range *r = bert_range_create(in, len);
  if(!r) {
    perror("bert_range_create");
    return;
  }

  printf("<<");
  for(size_t i = 0; i < len-1; ++i) {
    printf("%d,", in[i]);
  }
  printf("%d>>\n", in[len-1]);

  bert_byte_t const v = bert_get_version(r);
  printf("Version: %d\n", v);
  bert_type_t const t = bert_get_type(r);
  printf("Type: %d\n", t);
}

int main() {
  {
    byte_t buf[] = { 131, 109, 0, 0, 0, 5, 0xA, 0xB, 0xC, 0xD, 0xE };
    test(buf, sizeof(buf));
  }
}
