#include <math.h>
#include <stdint.h>
#include <stdio.h>

struct navigation_item {
  double value;
  int exp;
};

struct navigation_data {
  char satellite[3];
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  struct navigation_item items[15];
};

struct navigation_data parse(FILE *fp) {

  char *navHeaderLine = "%s %d %d %d %d %d %d %lfD%d %lfD%d %lfD%d";
  char *navLine = " %lfD%d %lfD%d %lfD%d %lfD%d";
  struct navigation_data nd;
  int line = -1;

  if (fscanf(fp, navHeaderLine, nd.satellite, &nd.year, &nd.month, &nd.day,
             &nd.hour, &nd.minute, &nd.second, &nd.items[0].value,
             &nd.items[0].exp, &nd.items[1].value, &nd.items[1].exp,
             &nd.items[2].value, &nd.items[2].exp) == EOF) {
    return nd;
  }

  while (++line,
         line < 3 &&
             fscanf(fp, navLine, &nd.items[3 + line * 4].value,
                    &nd.items[3 + line * 4].exp, &nd.items[4 + line * 4].value,
                    &nd.items[4 + line * 4].exp, &nd.items[5 + line * 4].value,
                    &nd.items[5 + line * 4].exp, &nd.items[6 + line * 4].value,
                    &nd.items[6 + line * 4].exp) != EOF) {
  }

  return nd;
}

double item_value(const struct navigation_data *nd, const uint8_t item_index) {
  return nd->items[item_index].value * pow(10, nd->items[item_index].exp);
}

struct observation {
  char *satellite;
  int a;
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Argument count is invalid. Need %d but got %d", 2, argc);
    return 1;
  }

  FILE *fptr;
  fptr = fopen(argv[1], "r");
  char buf[100];
  int line = -1;
  int headerLength = 5;

  while (++line, line < headerLength && fgets(buf, 100, fptr)) {
    printf("%s", buf);
  }

  struct navigation_data nd = parse(fptr);
  printf("%s %d %d\n", nd.satellite, nd.year, nd.month);
  for (int i = 0; i < 15; ++i)
    printf("%lf\n", item_value(&nd, i));

  fclose(fptr);
}
