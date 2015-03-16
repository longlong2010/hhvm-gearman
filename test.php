<?php
$c = new GearmanClient();
$c->addServer('127.0.0.1');
$h = $c->doBackground('abc', 'abc');
//while(true)
var_dump($c->jobStatus($h));
//exit;
$w = new GearmanWorker();
$w->addServer('127.0.0.1');
var_dump($w->addFunction('abc', function() {
	echo "123\n";
}, $ctx, 0));
while (true) {
	$w->work();
}
