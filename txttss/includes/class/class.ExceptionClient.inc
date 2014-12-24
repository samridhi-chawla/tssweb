<?php

class ExceptionClient extends Exception {
	function __toString() {
		return "Client error ".$this->getCode()." :: ".$this->getMessage();
	}
}

?>