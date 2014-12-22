var sectionScroll = {
    debouncer: function (func, timeout) {
        var timeoutID , timeout = timeout || 500;
        return function () {
            var scope = this , args = arguments;
            clearTimeout( timeoutID );
            timeoutID = setTimeout( function () {
                func.apply( scope , Array.prototype.slice.call( args ) );
            } , timeout );
            return false;
        };
    },
    scrollToSection : function (target) {
        //console.log(target);
        $('html, body').animate({
            scrollTop: $(target).offset().top
        }, 1500);
        $(".sectionScroll.active").removeClass("active");
        $(target).addClass("active");
        //window.location.hash = $(target).get(0).id;
    },
    scrollToNextSection : function (index){
        if(typeof index == "undefined")
            index = 0;
        var next = $(".sectionScroll.active").nextAll(".sectionScroll").get(index);
        if(typeof next != "undefined")
            this.scrollToSection($(next));
    },
    scrollToPrevSection : function(index){
        if(typeof index == "undefined")
            index = 0;
        var prev = $(".sectionScroll.active").prevAll(".sectionScroll").get(index);
        if(typeof prev != "undefined")
            this.scrollToSection($(prev));
    },
    init: function() {
        var i = 0;
        $(".sectionScroll").map(function() {
            i++;
            this.id = "sectionScroll"+i;
        });

        
            this.scrollToSection("#sectionScroll1");
        
        
        window.addEventListener("wheel", this.debouncer(function(event){
            if(event.deltaY >= 0) {
                sectionScroll.scrollToNextSection();
            } else {
                sectionScroll.scrollToPrevSection();
            }
            event.preventDefault();
            return false;
        }));
        window.addEventListener("keydown", function(event){
            if(event.keyCode == 38) {
                event.preventDefault();
                sectionScroll.scrollToPrevSection();
            } else if(event.keyCode == 40) {
                event.preventDefault();
                sectionScroll.scrollToNextSection();
            }
            return false;
        });
    }
}

$(document).ready(function(){
    sectionScroll.init();
});