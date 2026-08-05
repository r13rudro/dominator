#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define print "print"
#define var_in "varin"
#define scan "scan"
#define var_out "varout"
#define if_else "jodi"
#define loop "loop"
FILE *f = fopen("rud.sec", "r");
int isLoop = 0;

char var[100][100];
int in = -1;
int val[100];
int line_in(char *line, char last, FILE *source)
{
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

  // for (; ; i++)
  // {
  //   /* code */

  // }
  // printf("%d\n", i);
  return i;
}
// int call(FILE *f,char line[])
// { int jk=*fgets(line,1024,f);//printf("%d\n", jk);
//   return jk;

// }
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
  // for(;line[i]==' ';i++){}
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
int condition(char *line, int *i)
{
  int l, r, ans = 0, b = *i;
  char op;
  skip(line, &b);
  if (isdigit(line[b]))
    l = num_in(line, &b);
  if (isalpha(line[b]))
    l = val[var_finder(line, &b)];
  skip(line, &b);
  op = line[b];
  b++;
  // printf("%c\n", op);
  skip(line, &b);
  // printf("%c\n", line[b]);
  if (isdigit(line[b]))
    r = num_in(line, &b);
  if (isalpha(line[b]))
    r = val[var_finder(line, &b)];
  // printf("%d\n", l);
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
  // printf("%d\n", b);
  *i = b;
  return ans;
}
void lexer(char line[], int i);
void multiLine(char line[],int lineCount){
      line_in(line, ',', f);
      // printf("%c",line[d]);
      // i=0;
      char multiLine[30][1024];
      int b = 0, d = 0, f = 0;
      while (line[d] != ',')
      {
        int indecator = 0;
        while (line[d] != '\n')
        {
          if (line[d] == ',')
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
            // printf("%c", multiLine[m][k]);
            line[k] = multiLine[m][k];
          }
          lexer(line, 0);
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
  // printf("%s",key);
  if (strcmp(key, print) == 0)
  {
    while (line[i] != '"')
    {
      i++;
    }
    i++;
    for (; line[i] != '"'; i++)
    {
      printf("%c", line[i]);
    }
    printf("\n");
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
    val[in] = num_in(line, &i);
    // printf("%d\n", val[in]);
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
  else if (strcmp(key, var_out) == 0)
  {
    skip(line, &i);
    printf("%d\n", val[var_finder(line, &i)]);
  }
  // edit
  else if (strcmp(key, if_else) == 0)
  {
    // int c=condition(line,&i);
    // c=1;
    // printf("%d\n", c);
    if (condition(line, &i))
    {
      for (; line[i] != ','; i++)
      {
      }
      i++;
      lexer(line, i);
      if (line[i] == '\n')
      {
        line_in(line, '\n', f);
      }
    }
    else
    {
      // printf("%c\n", line[i]);
      while (i++)
      {
        if (line[i] == ',' && line[i + 1] == ',')
          break;
      }
      // for(;line[i]!=',' && line[i+1]!=',';i++){}
      i += 3;
      lexer(line, i);
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
      // i++;
      // while (line[i] != ",")
multiLine(line, c);
      // printf("%c",line[1]);
      // while ()
      // {
      // }

      // int a = 0, b = 0;
      // while (c--)
      // {
      //   // line_in(multiLine, '', f);
      //   while (line[i] != ",")
      //   {

      //   }
      //   lexer(multiLine, 0);
      // }
      // printf("%c",line[i]);
      isLoop = 0;
    }
    else
    {
      // int c=condition(line,&i);
      // printf("%d\n", c);

      skip(line, &i);
      isLoop = 1;
    again:
      // printf("%d\n", i);
      lexer(line, i);
      val[0]++;
      if (condition(line, &i) == 1)
      {
        goto again;
      }

      isLoop = 0;
    }
  }
  // printf("%c",line[i+3]);
  if (isLoop == 0)
  {
    i += 100;
    while (--i)
    {
      line[i] = ' ';
    }
  }
}

int main()
{
  // FILE *f = fopen("rud.sec", "r");
  char line[10240];

  // while(fgets(line,1024,f)){
  //   lexer(line,0);
  // }
  // fgets(line,1024,f);

  // printf("%c", line[2]);
  // char m=*f;

  while (line_in(line, '\n', f))
  {
    lexer(line, 0);
  }
}