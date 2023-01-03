#define DAY Day17

// To simplify the code organization, each day only as a cpp file, not a .h file.
// So we have to import it via "extern".
namespace Day1 { extern void Both(); }
namespace Day2 { extern void Both(); }
namespace Day3 { extern void Both(); }
namespace Day4 { extern void Both(); }
namespace Day5 { extern void Both(); }
namespace Day6 { extern void Both(); }
namespace Day7 { extern void Both(); }
namespace Day8 { extern void Both(); }
namespace Day9 { extern void Both(); }
namespace Day10 { extern void Both(); }
namespace Day11 { extern void Both(); }
namespace Day12 { extern void Both(); }
namespace Day13 { extern void Both(); }
namespace Day14 { extern void Both(); }
namespace Day15 { extern void Both(); }
namespace Day16 { extern void Both(); }
namespace Day17 { extern void Both(); }
namespace Day18 { extern void Both(); }
namespace Day19 { extern void Both(); }
namespace Day20 { extern void Both(); }
namespace Day21 { extern void Both(); }
namespace Day22 { extern void Both(); }
namespace Day23 { extern void Both(); }
namespace Day24 { extern void Both(); }
namespace Day25 { extern void Both(); }

int main()
{
#ifdef DAY
    DAY::Both();
#else
    Day1::Both();
    Day2::Both();
    Day3::Both();
    Day4::Both();
    Day5::Both();
    Day6::Both();
    Day7::Both();
    Day8::Both();
    Day9::Both();
    Day10::Both();
    Day11::Both();
    Day12::Both();
    Day13::Both();
    Day14::Both();
    Day15::Both();
    Day16::Both();
    Day17::Both();
    Day18::Both();
    Day19::Both();
    Day20::Both();
    Day21::Both();
    Day22::Both();
    Day23::Both();
    Day24::Both();
    Day25::Both();
#endif
}
