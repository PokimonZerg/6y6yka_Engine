# Run all tests
use strict;
use warnings;
use diagnostics;
use Win32::Console::ANSI;
use Term::ANSIColor;

my $test_count = 0;                 # num tests
my $fail = 0;                       # bad tests
my $win = 0;                        # passed tests

opendir(DIR, "test\\");
open(FILE, ">", "test_log.txt");

chdir "test/";                      # change working dir

foreach my $cur_file (readdir(DIR)) # get all files in test dir
{	
	
	if($cur_file =~ /.exe/)         # run all exe files
	{
		print(color("reset"), $test_count, ") Running ", $cur_file, "...");

		system("$cur_file" );       # run it

		if(($? >> 8) == 0)          # get program result
		{
			print(color("green"), "It's work! \n", color("reset"));
			print FILE ($test_count, ") ", $cur_file, " - It's work! \n");
			$win++;
		}
		else
		{
			print(color("red"), "Test error :-( \n", color("reset"));
			print FILE ($test_count, ") ", $cur_file, " - Test error :-( \n");
			$fail++;
		}
		
		$test_count++;
	}
}

print("Result: $win - win, and $fail - fail \n");
print FILE ("Result: $win - win, and $fail - fail \n");
system("pause");

close(FILE);
closedir(DIR);

__END__