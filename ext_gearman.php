<?hh

<<__NativeData("GearmanClientData")>>
class GearmanClient {
	<<__Native>>
	public function __construct(): void;

	<<__Native>>
	public function addServer(?string $host = '127.0.0.1', int $port = 4730): bool;
	
	<<__Native>>
	public function doNormal(string $function_name, string $workload, ?string $unique = null): string;
}

<<__NativeData("GearmanWorkerData")>>
class GearmanWorker {
	<<__Native>>
	public function __construct(): void;

	<<__Native>>
	public function work(): bool;
}
