<?php

class ExceptionDisplay extends Exception {
	function __toString() {
		return "Display error ".$this->getCode()." :: ".$this->getMessage();
	}
}

?>