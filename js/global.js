$(document).ready(function(){
      $("article img").each(function(){
        $(this).replaceWith("<a href='" + $(this).attr('src')+"' data-lightbox='倾旋'>"+
          "<img src='" + $(this).attr('src') +"' class='" + $(this).attr('class') +"' alt='" 
          + $(this).attr('alt') + "' title='" + $(this).attr('title') + "'"
          + " /></a>");
      });
});
