<?php

class ExceptionDatabase extends Exception {
	function __toString() {
		return "Database error ".$this->getCode()." :: ".$this->getMessage();
	}
}

?>