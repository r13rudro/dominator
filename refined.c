#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define print   "lekh"
#define var_in  "dhuk"
#define scan    "dhukbe"
#define var_out "bero"
#define if_else "jodi"
#define loop    "loop"

FILE *f = NULL;
int isLoop = 0;
char var[100][100];
int in = -1;
int val[100];

int line_in(char *line, char last, FILE *source)
{
    int i = 0;
    do {
        int c = fgetc(source);
        if (c == -1) break;
        line[i] = (char)c;
        i++;
    } while (line[i - 1] != last);
    return i;
}

int num_in(char *line, int *i)
{
    int out = 0, m = 1, j;
    for (; isdigit((unsigned char)line[*i]); (*i)++);
    for (j = (*i) - 1; isdigit((unsigned char)line[j]); j--) {
        out += (line[j] - '0') * m;
        m *= 10;
    }
    return out;
}

void skip(char *line, int *i)
{
    for (; *(line + (*i)) == ' '; (*i)++) {}
}

int var_finder(char *line, int *i)
{
    int b = *i, k;
    char name[100];
    int j = 0;
    while ((line[b] >= 'A' && line[b] <= 'Z') ||
           (line[b] >= 'a' && line[b] <= 'z') ||
           (line[b] >= '0' && line[b] <= '9') ||
           line[b] == '_') {
        name[j++] = line[b++];
    }
    name[j] = '\0';
    for (k = 0; k <= in; k++) {
        if (strcmp(var[k], name) == 0) {
            *i = b;
            return k;
        }
    }
    *i = b;
    return 0;
}

int condition(char *line, int *i)
{
    int l = 0, r = 0, ans = 0, b = *i;
    char op;
    skip(line, &b);
    if (isdigit((unsigned char)line[b])) l = num_in(line, &b);
    else if (isalpha((unsigned char)line[b])) l = val[var_finder(line, &b)];
    skip(line, &b);
    op = line[b]; b++;
    skip(line, &b);
    if (isdigit((unsigned char)line[b])) r = num_in(line, &b);
    else if (isalpha((unsigned char)line[b])) r = val[var_finder(line, &b)];

    switch (op) {
        case '<': if (l < r)  ans = 1; break;
        case '>': if (l > r)  ans = 1; break;
        case '=': if (l == r) ans = 1; break;
        case '!': if (l != r) ans = 1; break;
        default: break;
    }
    *i = b;
    return ans;
}

void lexer(char line[], int i)
{
    char key[10];
    int j = 0;
    skip(line, &i);
    for (; line[i] != ' '; i++, j++) key[j] = line[i];
    key[j] = '\0';

    if (strcmp(key, print) == 0) {
        while (line[i] != '"') i++;
        i++;
        for (; line[i] != '"'; i++) printf("%c", line[i]);
        printf("\n");
    }
    else if (strcmp(key, var_in) == 0) {
        in++;
        skip(line, &i);
        for (j = 0; line[i] != ' '; i++, j++) var[in][j] = line[i];
        i++;
        val[in] = num_in(line, &i);
    }
    else if (strcmp(key, scan) == 0) {
        in++;
        skip(line, &i);
        for (j = 0; line[i] != ' '; i++, j++) var[in][j] = line[i];
        scanf("%d", &val[in]);
    }
    else if (strcmp(key, var_out) == 0) {
        skip(line, &i);
        printf("%d\n", val[var_finder(line, &i)]);
    }
    else if (strcmp(key, if_else) == 0) {
        int cond_result = condition(line, &i);
        int if_start, else_pos, if_end;
        char if_branch[1024], else_branch[1024];

        while (line[i] != ',' && line[i] != '\0' && line[i] != '\n') i++;
        if (line[i] == ',') i++;
        if_start = i;
        else_pos = -1;

        while (line[i] != '\0' && line[i] != '\n') {
            if (line[i] == ',' && line[i + 1] == ',') {
                else_pos = i;
                break;
            }
            i++;
        }

        if (cond_result) {
            if_end = (else_pos != -1) ? else_pos : i;
            for (j = 0, i = if_start; i < if_end; i++) if_branch[j++] = line[i];
            if_branch[j] = '\0';
            lexer(if_branch, 0);
        } else if (else_pos != -1) {
            int else_start = else_pos + 2, k;
            for (j = 0, k = else_start; line[k] != '\0' && line[k] != '\n'; k++)
                else_branch[j++] = line[k];
            else_branch[j] = '\0';
            lexer(else_branch, 0);
        }
    }
    else if (strcmp(key, loop) == 0) {
        skip(line, &i);
        if (isdigit((unsigned char)line[i])) {
            int c = num_in(line, &i);
            char body[10240];
            int k;
            skip(line, &i);
            for (j = 0; line[i] != '\0' && line[i] != '\n'; j++) body[j] = line[i++];
            body[j] = '\0';
            isLoop = 1;
            for (k = 0; k < c; k++) lexer(body, 0);
            isLoop = 0;
        } else {
            int cond_start = i, cond_end, ci = 0;
            char cond[256], body[10240];
            int bi = 0;

            while (line[i] != ',' && line[i] != '\0' && line[i] != '\n') i++;
            cond_end = i;
            for (j = cond_start; j < cond_end; j++) cond[ci++] = line[j];
            cond[ci] = '\0';

            if (line[i] == ',') i++;
            while (line[i] != '\0' && line[i] != '\n') body[bi++] = line[i++];
            body[bi] = '\0';

            isLoop = 1;
            while (1) {
                int temp_i = 0;
                if (!condition(cond, &temp_i)) break;
                lexer(body, 0);
            }
            isLoop = 0;
        }
    }

    if (isLoop == 0) {
        i += 100;
        while (--i) { line[i] = ' '; }
    }
}

int main(int argc, char *argv[])
{
    char line[10240];
    if (argc < 2) {
        printf("Usage: %s <filename.sec>\n", argv[0]);
        return 1;
    }
    f = fopen(argv[1], "r");
    if (!f) {
        printf("File not found: %s\n", argv[1]);
        return 1;
    }
    while (line_in(line, '\n', f)) {
        lexer(line, 0);
    }
    fclose(f);
    return 0;
}