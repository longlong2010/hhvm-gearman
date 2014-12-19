<?php
$c = new GearmanClient();
$c->addServer('172.16.10.201');
var_dump($c->doNormal('abc', 'abc'));
$w = new GearmanWorker();
