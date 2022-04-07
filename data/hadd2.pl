#!/opt/star/sl64_gcc447/bin/perl

$ii = 0;
$jj=1;

$input ="";
$output="";

opendir(DIR, ".");
@files = grep(/\.root$/,readdir(DIR));
closedir(DIR);

foreach $file (@files){

	$space=" ";
	$input="$input$space$file";
	$ii++;
	if($ii==200)
	{
		$output="output$jj.root";
		$jj++;
		#print "$output $input\n";
		system("hadd $output $input");
		$input="";
		$ii=0;
	}
  
}

if($ii!=0 && $ii!=200)
{

		$last="last";
		$output="output$jj$last$ii.root";
		$jj++;
		system("hadd $output $input");
		$input="";
		$ii=0;
}
