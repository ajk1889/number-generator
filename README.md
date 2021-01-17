# number-generator
A sequential arbitary size file generator module for Test-suit server. It generates the following sequence of numbers and returns it to the client as if the client was downloading file.
```
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 ....
```
The module supports `Range` header (partial content) and therefore download can be resumed if it gets interrupted. The default file size is 100MB. File size and byte offset can be tweaked via HTTP GET parameter values.

### Accepted GET parameter values
- `start`: The byte position from which the sequence should be started (in bytes). **This is not the number from which sequence starts counting**<br/>
i.e. if `start` is set to 10, the generated sequence will be `5 6 7 8 9 10 11 12 13 14 ....`.<br/>
`start` `GET` parameter can be omitted to start the sequence generation from zero. 
- `end`: The byte position (exclusive) up to which sequence should be generated (in bytes).
If `end` is set to 11, the generated sequence will be `0 1 2 3 4 5` (first 11 characters of the sequence).<br/>
If `end` is omitted, end will be considered to be `104857600` (equivalent to `100MB`).
- `show`: If this key is present in GET parameters list (its value is irrelevant), the sequence will be displayed directly instead of initiating a download.
i.e. `Content-Disposition` header will not be passed to the client.

When `start` and `end` are passed together, the result will contain characters of the sequence starting from position `start` up to position `end` (exclusive).<br/>
For example, for the request `/generate?start=20&end=40` the result will be `10 11 12 13 14 15 16`

`start` value should never be greater than `end` value. The behaviour is not defined in that case.

### Range header support
This module supports Range headers (Single range only).
The download can be stopped and resumed without requiring to start from beginning.

### Data share mode
Test-suit supports `data: /path/to/file` and `data: inline` methods for reading response from modules.
This module uses `data: inline` mode to improve performance when client requests huge ranges which might not fit in the available storage space.

### Sequence generation overhead
The sequence generation algorithm has constant time and space complexity.
i.e. this module is capable to generate sequence from any given byte offset without starting to count from beginning.

Therefore it is safe to input any value in the range of `unsigned long long` (0-18446744073709551616) for `start` and `end` GET parameters.
   
