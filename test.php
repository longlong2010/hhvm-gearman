<?php
$c = new GearmanClient();
$c->addServer('127.0.0.1');
var_dump($c->doBackground('abc', 'abc'));
$w = new GearmanWorker();
$w->addServer('127.0.0.1');
$w->addFunction('abc', function() {
	echo "123\n";
}, $c, 0);
while (true) {
	$w->work();
}
