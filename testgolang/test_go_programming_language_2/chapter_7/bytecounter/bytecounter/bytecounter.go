package bytecounter

type ByteCounter int

func (c *ByteCounter) Write(p []byte) (int, error) {
	*c += ByteCounter(len(p)) //Convert int to ByteCounter
	return len(p), nil
}
