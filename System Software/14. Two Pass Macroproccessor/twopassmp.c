#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cat(FILE * fp) {
	char line[50];
	rewind(fp);
	while(! feof(fp)) {
		fscanf(fp, "%[^\n]\n", line);
		printf("%s\n", line);
	}
}

void pass_one() {
  FILE *input, *namtab, *deftab;
  char label[20], opcode[20], operand[20];

  if (!(input = fopen("input.txt", "r"))) {
    printf("Please make sure input.txt exists\n");
    exit(0);
  }

  namtab = fopen("namtab.txt", "w+");
  deftab = fopen("deftab.txt", "w+");

  do {
    fscanf(input, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "MACRO") == 0) {
      fprintf(namtab, "%s\n", label);
      fprintf(deftab, "%s %s\n", label, operand);
    }

    else
      fprintf(deftab, "%s %s\n", opcode, operand);
	  
  } while (strcmp(opcode, "MEND"));

  printf("Pass one completed\n");
  
  printf("Output of namtab.txt: \n");
  cat(namtab);
  printf("Output of deftab.txt: \n");
  cat(deftab);

  fclose(input);
  fclose(namtab);
  fclose(deftab);
}

void pass_two() {
  FILE *input, *deftab, *namtab, *argtab, *output;
  char label[20], opcode[20], operand[20], macro_name[20], *token;
  char deftab_opcode[20], deftab_operand[20], macro_args[20][20],
      operand_sub[20];
  size_t macro_args_idx = 0;

  if (!(input = fopen("input.txt", "r")) ||
      !(deftab = fopen("deftab.txt", "r")) ||
      !(namtab = fopen("namtab.txt", "r"))) {
    printf("Pass two failed - Verify the output of pass one\n");
  }

  argtab = fopen("argtab.txt", "w+");
  output = fopen("output.txt", "w+");

  fscanf(namtab, "%s", macro_name);

  do {
    fscanf(input, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "MACRO") == 0) {
      token = strtok(operand, ",");
      while (token != NULL) {
        strcpy(macro_args[macro_args_idx++], token);
        token = strtok(NULL, ",");
      }

      while (strcmp(opcode, "MEND") != 0)
        fscanf(input, "%s %s %s", label, opcode, operand);
    }

    token = NULL;

    if (strcmp(opcode, macro_name) == 0) {
      token = strtok(operand, ",");
      while (token != NULL) {
        fprintf(argtab, "%s\n", token);
        token = strtok(NULL, ",");
      }

      token = NULL;

      fscanf(deftab, "%s %s", deftab_opcode, deftab_operand);
      while (strcmp(deftab_opcode, "MEND") != 0) {
		fprintf(output, "- %s ", deftab_opcode);
        token = strtok(deftab_operand, ",");
        while (token != NULL) {
          if (token[0] == '&') {
            rewind(argtab);
            macro_args_idx = 0;
            while (strcmp(macro_args[macro_args_idx++], token)) {
              fscanf(argtab, "%s", operand_sub);
            }
            fscanf(argtab, "%s", operand_sub);
			fprintf(output, "%s,", operand_sub);
          } else {
			  fprintf(output, "%s,", token);
		  }
          token = strtok(NULL, ",");
        }
		fseek(output, -1, SEEK_CUR); // To remove the trailing comma
		fprintf(output, "\n");
        fscanf(deftab, "%s %s", deftab_opcode, deftab_operand);
      }
    } else {
		if(strcmp(opcode, "MEND") != 0)
		fprintf(output, "%s %s %s\n", label, opcode, operand);
	}

  } while (strcmp(opcode, "END"));

  printf("\nPass two completed\n");

  printf("Output of argtab.txt");
  cat(argtab);
  printf("Output of output.txt\n");
  cat(output);

  fclose(argtab);
  fclose(deftab);
  fclose(namtab);
  fclose(input);
  fclose(output);
}

void main() {
  pass_one();
  pass_two();
}
