#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc > 2) {
    fprintf(stderr, "Usage: %s [optional brightness value]\n", argv[0]);
    exit(1);
  }

  FILE *fp = fopen("/sys/class/backlight/intel_backlight/max_brightness", "r");

  if (fp == NULL) {
    fprintf(stderr, "Couldn't open max_brightness for reading. Are you sure you're using an Intel card? If yes, please check if your kernel exposes '/sys/class/backlight/intel_backlight'.\n");
    exit(1);
  }

  char buffer[32];
  if (fgets(buffer, 32, fp) == NULL) {
      fprintf(stderr, "Couldn't read from max_brightness.\n");
      fclose(fp);
      exit(1);
  }

  fclose(fp);

  long max = strtol(buffer, NULL, 10);

  int input = -1;
  if (argc == 2) {
    input = strtol(argv[1], NULL, 10);

    if (input <= 0 || input > 100) {
      fprintf(stderr, "Value should be between 1 and 100!\n");
      exit(1);
    }
  }


  if (input == -1) {
    printf("Enter a value between 1 and 100: ");

    while (!scanf("%d", &input) || input <= 0 || input > 100) {
      while (fgetc(stdin) != '\n') // Read until a newline is found
          ;
      printf("Bad input!\nEnter a value between 1 and 100: ");
    }
  }

  long value = (max/100.0)*input;

  if (argc == 1) printf("%d%% = %ld\n", input, value);


  fp = fopen("/sys/class/backlight/intel_backlight/brightness", "w");
  if (fp == NULL) {
    fprintf(stderr, "Couldn't open brightness file for writing. Check if you're running as root!\n");
    exit(1);
  }

  fprintf(fp, "%ld\n", value);
  fclose(fp);

  return 0;
}
