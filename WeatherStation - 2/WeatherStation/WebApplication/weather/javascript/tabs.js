$(function() {
  $('article.tabs section > h3').click(function() {
    $('article.tabs section').removeClass('current');
    $(this)
    .closest('section').addClass('current');
  });
});
