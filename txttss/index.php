<?php

// $_REQUEST['txtweb-message'] = "";
// $_REQUEST["txtweb-id"] = "c3e112e6-8ca0-46bc-8da5-86158a105f06";
// $_REQUEST['txtweb-mobile']="5dee1c0b-7a71-426e-a5c9-4788e79368f4";
// $_REQUEST["txtweb-verifyid"] = "1e375a0927139100164f15d3ff29712e4a71c818b307f30fe624c6ba0b961ddb0780a6e719df5c054580e12bb2e69c2af81afc3b613ac8816c29b2940f03bebf35d92b406171e453c8b8ffc8496179bdc7dcb445d3c078d7bdfbeb5669dc6a99cae18ea77d3d077c";
// $_REQUEST["txtweb-aggid"] = 21000;
// $_REQUEST["txtweb-protocol"] = 2100;

$requests = print_r($_REQUEST, true);
$requests .= print_r($_GET, true);
file_put_contents("requests.txt", $requests);

function __autoload($class_name) {
	@include "includes/class/class.".$class_name.".inc";
	@include "../include/class/class.".$class_name.".inc";
}
if(isset($_GET) && isset($_REQUEST)) {
	try{
		$fetch = new Fetch($_REQUEST, $_GET);
		$output = $fetch->display();
		echo $output;
	} catch(Exception $e) {
		echo '<html>
<head>
  <title> TSS Text Services </title>
  <meta name="txtweb-appkey" content="73684a1f-0c3b-4144-8b0b-072950eb2b03" />
</head>
<body>';
		if($e instanceof ExceptionClient) {
			echo $e;
		} else if($e instanceof ExceptionDatabase) {
			echo $e;
		} else if($e instanceof ExceptionDisplay) {
			echo $e;
		} else if($e instanceof ExceptionFetch) {
			echo $e;
		} else {
			echo "unknown error ".$e->getCode()." :: ".$e->getMessage();
		}
echo '</body>
</html>';
	}
}

?>