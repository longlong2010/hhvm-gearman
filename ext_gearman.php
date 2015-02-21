<?hh

<<__NativeData("GearmanClientData")>>
class GearmanClient {
	<<__Native>>
	public function __construct(): void;

	<<__Native>>
	public function addServer(?string $host = '127.0.0.1', int $port = 4730): bool;
	
	<<__Native>>
	public function doNormal(string $function_name, string $workload, ?string $unique = null): string;

	<<__Native>>
	public function doHigh(string $function_name, string $workload, ?string $unique = null): string;
	
	<<__Native>>
	public function doBackground(string $function_name, string $workload, ?string $unique = null): string;
	
	<<__Native>>
	public function doHighBackground(string $function_name, string $workload, ?string $unique = null): string;
}

<<__NativeData("GearmanWorkerData")>>
class GearmanWorker {
	<<__Native>>
	public function __construct(): void;

	<<__Native>>
	public function addServer(?string $host = '127.0.0.1', int $port = 4730): bool;
	
	<<__Native>>
	public function addFunction(string $function_name, mixed $function, mixed &$context = null, int $timeout = 0): bool;
	
	<<__Native>>
	public function work(): bool;
}
