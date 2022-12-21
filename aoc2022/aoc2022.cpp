#define DAY Day14

// To simplify the code organization, each day only as a cpp file, not a .h file.
// So we have to import it via "extern".
namespace DAY
{
    extern void Both();
}

int main()
{
    DAY::Both();
}
