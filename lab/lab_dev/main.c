#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

/* 1. type name;
 * 2. type name(type name, ...); {}
 * 3. & && | ||
 * 4. if (expr)
 *      stmnt
 *    else
 *      stmnt
 *
 *    while (expr)
 *      stmnt
 *
 *    for (expr1; expr2; expr3)
 *      stmnt
 *
 *    do
 *      stmnt
 *    while(expr)
 *
 *    expr1 ? expr2 : expr3;
 * 5. int a, *pa
 *
 * ---------------------------
 *
 * 5.5. I/O: printf, scanf, getchar, putchar, fgets, fread, fwrite, etc.
 * 5.5.5. __asm__
 *
 * ---------------------------
 *
 * 6. struct, union, enum, typedef
 * 7. int a[10]
 * 8. const char *str = "Hello!"
 * 9. std::list, std::queue, std::stack, std::deque
 * 10. std::array, std::vector, std::string
 * 11. std::set, std::map, std::multiset, std::multimap, std::unordered_map, std::unordered_multimap, ...
 * 12. std::sort, ...
 * 13. ...
 * 14. Problems
 */

static const char * const string = "Hello, World!\n";
static char *pstring = NULL;
#define len_string sizeof("Hello, World!\n") - 1

#define len_string2 (1024)
char string2[len_string2];


main(int aa, int bb)
{
/*    syscall(SYS_WRITE, 1, string, len_string);
    syscall(SYS_EXIT, 0);*/

/*    pstring = string;


    string = "Hello, aaaa!!!";*/

    int a;
    int b = 5;

    write(1, string, len_string);

/*    exit(0);*/

    if ( 2 + 2 >= 5 )
    {

    }
    else
    {

    }

    switch ( expr )
    {
        case 1:
            break;
        default:
            break;
    }

    return 0;
}
