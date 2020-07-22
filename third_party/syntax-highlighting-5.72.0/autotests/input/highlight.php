<?
/* This is a pseudo PHP file to test Kate's PHP syntax highlighting. */
# TODO: this is incomplete, add more syntax examples!
# this is also a comment.
// Even this is a comment
function test($varname) {
	return "bla";	# this is also a comment
}

?>

<?php echo("hello test"); ?>

<html>
	<? print "<title>test</title>"; ?>

	<!-- CSS -->
	<style>
		.inputText {
			width: <?php echo $width; ?>px;
			text-indent: 10px;
		}
	</style>

	<!-- JavaScript -->
	<script>
		var some_js_var = <?php echo $somevar; ?> ;
		<?php echo 'alert("Hello there.");'; ?>
	</script>
	<!-- JavaScript React -->
	<script type="text/babel">
		<Hello> <?php echo("Hello, hello!"); ?> </Hello> <?php /* aaa */ ?> <div></div>
		function a(i) {
			<?php echo "var j = 1;"; ?>
			return <p>{ i + j }</p>;
		}
	</script>
	<!-- TypeScript -->
	<script type="text/typescript">
		<?php $timestamp = time(); ?>
		class DateTime {
			info: string;
			constructor() { this.info = <?php echo(date("F d, Y h:i:s", $timestamp)); ?>; }
			get() { return this.info; }
		}
	</script>
	<!-- MustacheJS -->
	<script type="x-tmpl-mustache">
		{{! <?php print "comment"; ?> }}
		{{#movie}}
			<div>
				<h1>{{title}}</h1>
				<img src="{{poster}}" alt="{{title}}"/>
				<?php echo $movierating; ?> - {{ratings.critics_rating}}
			</div>
		{{/movie}}
	</script>

</html>

<?php
$var = <<<DOOH
This is the $string inside the variable (which seems to be rendered as a string)
It works well, I think.
DOOH

# bug 382527
throw new ParserException("Test {$this->some_var[$index]}\nin {$this->file} is missing.\nThis is bad.");
?>
