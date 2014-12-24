<?php

class ExceptionFetch extends Exception {
	function __toString() {
		return "Fetch error ".$this->getCode()." :: ".$this->getMessage();
	}
}

?>