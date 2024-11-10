
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void passone(char label[20],char opcode[20],char operand[20],char code[10],char mnemonic[5]);
void passtwo(char label[20],char opcode[20],char operand[20],char code[10],char mnemonic[5],char symbol[10]);

int main(){
    char label[20],opcode[20],operand[20];
    char code[10],mnemonic[5];
    char symbol[10];
    passone(label,opcode,operand,code,mnemonic);
    passtwo(label,opcode,operand,code,mnemonic,symbol);
    return 0;
    
}
void passone(char label[20],char opcode[20],char operand[20],char code[10],char mnemonic[5]){
    int locctr,start,length;
    FILE *fp1,*fp2,*fp3,*fp4,*fp5;
    fp1 = fopen("input.txt","r");
    fp2 = fopen("optab.txt","r");
    fp3 = fopen("symtab.txt","w");
    fp4 = fopen("intermediate.txt","w");
    fp5 = fopen("length.txt","w");

    fscanf(fp1,"%s\t%s\t%s",label,opcode,operand);
    if(strcmp(opcode,"START")==0)
    {
        start = atoi(operand);
        locctr = start;
        fprintf(fp4,"\t%s\t%s\t%s\n",label,opcode,operand);
        fscanf(fp1,"%s\t%s\t%s",label,opcode,operand);
    }else{
        locctr=0;
    }
    while (strcmp(opcode,"END")!=0)
    {
        fprintf(fp4,"%d\t%s\t%s\t%s\n",locctr,label,opcode,operand);
        if (strcmp(label,"**")!=0)
        {
            fprintf(fp3,"%s\t%d\n",label,locctr);
        }
        fscanf(fp2,"%s\t%s",code,mnemonic);
        while (strcmp(code,"END")!=0)
        {
            if(strcmp(code,opcode)!=0)
            {
                locctr+=3;
                break;
            }
            fscanf(fp2,"%s\t%s",code,mnemonic);
        }
        if(strcmp(opcode,"WORD")==0){
            locctr+=3;
        }else if(strcmp(opcode,"RESW")==0)
        {
            locctr+=((atoi(operand))*3);
        }else if(strcmp(opcode,"BYTE")==0)
        {
            locctr+=1;
        }else if(strcmp(opcode,"RESB")==0)
        {
            locctr+=atoi(operand);
        }
        fscanf(fp1,"%s\t%s\t%s",label,opcode,operand);
    }
    fprintf(fp4,"%d\t%s\t%s\t%s\n",locctr,label,opcode,operand);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    length = locctr - start;
    fprintf(fp5, "%d\n", length);
    fclose(fp5);
}
void passtwo(char label[20],char opcode[20],char operand[20],char code[10],char mnemonic[5],char symbol[10]){
    char a[10], ad[10], ch;
  int st, diff, i, address=0, add, len, actual_len, finaddr, prevaddr, j = 0;
  FILE *fp1, *fp2, *fp3, *fp4,*fp5;
  fp1 = fopen("assmlist.txt", "w");
  fp2 = fopen("symtab.txt", "r");
  fp3 = fopen("intermediate.txt", "r");        
  fp4 = fopen("object.txt", "w");
  fp5 = fopen("optab.txt", "r");

  fscanf(fp3, "%s%s%s", label, opcode, operand);  

  while (strcmp(opcode, "END") != 0)  
  {
    prevaddr = address;
    fscanf(fp3, "%d%s%s%s", &address, label, opcode, operand);
  }
  
  finaddr = address;

  fclose(fp3);


  fp3 = fopen("intermediate.txt", "r");

  fscanf(fp3, "%s%s%s", label, opcode, operand);


  if (strcmp(opcode, "START") == 0)
  {
    fprintf(fp1, "\t%s\t%s\t%s\n", label, opcode, operand);
    fprintf(fp4, "H^%s^00%s^00%d\n", label, operand, finaddr);
    fscanf(fp3, "%d%s%s%s", &address, label, opcode, operand);
    st = address;
    diff = prevaddr - st;
       
    fprintf(fp4, "T^00%d^%d", address, diff);
  }


  while (strcmp(opcode, "END") != 0)
  {
    if (strcmp(opcode, "BYTE") == 0)
    {
      fprintf(fp1, "%d\t%s\t%s\t%s\t", address, label, opcode, operand);
      len = strlen(operand);
      actual_len = len - 3;
      fprintf(fp4, "^");
      for (i = 2; i < (actual_len + 2); i++)
      {
        sprintf(ad, "%04x", operand[i]);
        fprintf(fp1, "%s", ad);
        fprintf(fp4, "%s", ad);
      }
      fprintf(fp1, "\n");
    }
    else if (strcmp(opcode, "WORD") == 0)
    {
      
      sprintf(a, "%d", atoi(operand));
      fprintf(fp1, "%d\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, a);
      fprintf(fp4, "^00000%s", a);
    }
    else if ((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0))
      fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
    else
    {
      rewind(fp5);
      fscanf(fp5, "%s\t%s", mnemonic,code);
      while (strcmp(opcode,mnemonic ) != 0)
          fscanf(fp5, "%s\t%s",mnemonic,code);
      // while (strcmp(opcode, mnemonic[j]) != 0)
      //   j++;
      if (strcmp(operand, "COPY") == 0)
        fprintf(fp1, "%d\t%s\t%s\t%s\t%s0000\n", address, label, opcode, operand, code);
      else
      {
        rewind(fp2);
        fscanf(fp2, "%s%d", symbol, &add);
        while (strcmp(operand, symbol) != 0)
          fscanf(fp2, "%s%d", symbol, &add);
        fprintf(fp1, "%d\t%s\t%s\t%s\t%s%d\n", address, label, opcode, operand, code, add);
        fprintf(fp4, "^%s%d", code, add);
      
      }
    }
    fscanf(fp3, "%d%s%s%s", &address, label, opcode, operand);
  }


  fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
  fprintf(fp4, "\nE^00%d", st);

  printf("\nObject code generated successfully!\n");
}
