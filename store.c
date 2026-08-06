#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define print "lekh"
#define var_in "varin"
#define scan "scan"
#define if_else "jodi"
#define loop "loop"
FILE *f;
int isLoop = 0;
char condi[50];
char var[100][100];
int in = -1;
int val[100];
void lineFree(char *line)
{
  int i = 0;
  for (; i < 600; i++)
  {
    line[i] = '\0';
  }
  line[i + 1] = '\0';
}
int line_in(char *line, char last, FILE *source)
{
  lineFree(line);
  int i = 0;
  do
  {
    int c = fgetc(source);
    if (c == -1)
    {
      break;
      // i = 3;
    }
    line[i] = c;
    i++;
  } while (line[i - 1] != last);
  return i;
}
int num_in(char *line, int *i)
{
  int out = 0, m = 1;
  for (; isdigit(line[(*i)]); (*i)++)
    ;
  for (int j = (*i) - 1; isdigit(line[j]); j--)
  {
    out += (line[j] - '0') * m;
    m *= 10;
  }
  return out;
}
void skip(char *line, int *i)
{
  for (; *(line + (*i)) == ' '; (*i)++)
  {
  }
}
int var_finder(char *line, int *i)
{
  int b = *i;
  int out = 0;
  // line[b]!=' ' && line[b]!='\n' && line[b]!='\0'
  for (; line[b] >= 'A' && line[b] <= 'z'; b++)
  {
    if (var[out][b - (*i)] != line[b])
    {
      out++;
      b = *i;
      if (out > 99)
      {
        break;
      }
    }
  }
  *i = b;
  return out;
}
int math(int index, char *line, int *i)
{
  char op;
  int b = *i, n;
  skip(line, &b);
  op = line[b];
  b++;
  // printf("%c\n", op);
  skip(line, &b);
  // printf("%c\n", line[b]);
  if (isdigit(line[b]))
    n = num_in(line, &b);
  if (isalpha(line[b]))
    n = val[var_finder(line, &b)];
  int ans, ind = 1;
  switch (op)
  {
  case '+':
    ans = 1;
    break;
  case '-':
    ans = 1;
    break;
  case '*':
    ans = 1;
    break;
  case '/':
    ans = 1;
    break;
  case '%':
    ans = val[index] % n;
    break;

  default:
    ans = val[index];
    ind = 0;
    break;
  }
  if (ind == 1)
    *i = b;
  return ans;
}
int condition(char *line, int *i)
{
  int l, r, ans = 0, b = *i;
  char op;
  skip(line, &b);
  int start = b;
  if (isdigit(line[b]))
    l = num_in(line, &b);
  if (isalpha(line[b]))
    // l = val[var_finder(line, &b)];   //simple, without math
    l = math(var_finder(line, &b), line, &b);
  // printf("%d\n", l);
  skip(line, &b);
  op = line[b];
  b++;
  // printf("%c\n", op);
  skip(line, &b);
  // printf("%c\n", line[b]);
  if (isdigit(line[b]))
    r = num_in(line, &b);
  if (isalpha(line[b]))
    // r = val[var_finder(line, &b)];   //simple, without math
    r = math(var_finder(line, &b), line, &b);
  // printf("%d\n", l);
  int end = b;
  for (int p = start, q = 0; p < end + 1; q++, p++)
  {
    condi[q] = line[p];
  }

  switch (op)
  {
  case '<':
    if (l < r)
      ans = 1;
    break;
  case '>':
    if (l > r)
      ans = 1;
    break;
  case '=':
    if (l == r)
      ans = 1;
    break;
  case '!':
    if (l != r)
      ans = 1;
    break;

  default:
    break;
  }
  *i = b;
  return ans;
}
void printLine(char line[])
{
  for (int o = 0; o < 100; o++)
  {
    printf("%c", line[o]);
  }
  printf("\n");
  printf("\n");
}
void lexer(char line[], int i);
void multiLine(char line[], char last, int lineCount)
{
  char multiLine[30][1024];
  int b = 0, d = 0, f = 0;
  while (line[d] != last)
  {
    int indecator = 0;
    while (line[d] != '\n')
    {
      if (line[d] == last)
      {
        indecator = 1;
        break;
      }
      multiLine[b][f] = line[d];
      f++;
      d++;
    }
    multiLine[b][f] = '\0';
    if (indecator == 1)
      break;
    f = 0;
    b++;
    d++;
  }
  while (lineCount--)
  {
    for (int m = 0; m < b + 1; m++)
    {
      for (int k = 0; multiLine[m][k] != '\0'; k++)
      {
        line[k] = multiLine[m][k];
      }
      lexer(line, 0);
      lineFree(line);
    }
  }
}
void lexer(char line[], int i)
{
  skip(line, &i);
  char key[10];
  int j = 0;
  for (; line[i] != ' '; i++, j++)
  {
    key[j] = line[i];
  }
  key[j] = '\0';
  if (strcmp(key, print) == 0)
  {
    skip(line, &i);
    if (line[i] == '"')
    {
      i++;
      for (; line[i] != '"'; i++)
      {
        printf("%c", line[i]);
      }
      printf("\n");
    }
    else
    {
      printf("%d\n", val[var_finder(line, &i)]);
    }
  }
  else if (strcmp(key, var_in) == 0)
  {
    in++;
    skip(line, &i);

    for (int j = 0; line[i] != ' '; i++, j++)
    {
      var[in][j] = line[i];
    }
    i++;
    // int m=1;
    // while(isdigit(line[++i]));
    // while(isdigit(line[--i]))
    // {
    //   val[in]+=(line[i]-'0')*m;m*=10;
    // }
    skip(line, &i);
    val[in] = num_in(line, &i);
  }
  else if (strcmp(key, scan) == 0)
  {
    in++;
    skip(line, &i);

    for (int j = 0; line[i] != ' '; i++, j++)
    {
      var[in][j] = line[i];
    }
    scanf("%d", &val[in]);
    // while(isdigit(line[++i]));
    // int m=1;
    // while(isdigit(line[--i]))
    // {
    //   val[in]+=(line[i]-'0')*m;m*=10;
    // }
  }
  // else if (strcmp(key, var_out) == 0)
  // {
  //   skip(line, &i);
  //   printf("%d\n", val[var_finder(line, &i)]);
  // }
  else if (strcmp(key, if_else) == 0)
  {
    if (condition(line, &i))
    {
      line_in(line, ',', f);
      multiLine(line, ',', 1);
      line_in(line, '.', f);
    }
    else
    {
      line_in(line, ',', f);
      line_in(line, '.', f);
      multiLine(line, '.', 1);
    }
  }
  else if (strcmp(key, loop) == 0)
  {
    skip(line, &i);
    if (isdigit(line[i]))
    {
      int c = num_in(line, &i);
      skip(line, &i);
      isLoop = 1;
      line_in(line, '.', f);
      multiLine(line, '.', c);
      // for (int o = 0; o < 66; o++)
      // {
      //   /* code */
      //   printf("%c",line[o]);
      // }
      // printf("\n");
      isLoop = 0;
    }
    else
    {
      skip(line, &i);
      condition(line, &i);
      while (condition(condi, &i))
      {
        line_in(line, '.', f);
        multiLine(line, '.', 1);
      }
      //   isLoop = 1;
      // isLoop = 0;
    }
  }
  // if (isLoop == 0)
  // {
  //   i += 100;
  //   while (--i)
  //   {
  //     line[i] = ' ';
  //   }
  // }
}

int main()
{
  f = fopen("rud.sec", "r");
  char line[10240];
  // while(fgets(line,1024,f)){
  //   lexer(line,0);
  // }

  while (line_in(line, '\n', f))
  {
    lexer(line, 0);
  }
}