#!/opt/star/sl64_gcc447/bin/perl

$ii = 0;

$input ="";
$output="";

opendir(DIR, ".");
@files = grep(/\.root$/,readdir(DIR));
closedir(DIR);

foreach $file (@files){

	$space=" ";
	$input="$input$space$file";
	$ii++;
}

$output="output.root";
#print "$output $input\n";
system("hadd $output $input");
