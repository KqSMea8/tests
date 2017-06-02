package main

import (
	"fmt"
	"net/url"
	"strconv"
)

type LsCmd struct {
	Method string
	R      bool
	Args   []string
}

func (p *LsCmd) ToUrlParam() string {
	parameters := url.Values{}
	parameters.Add("method", p.Method)
	parameters.Add("r", strconv.FormatBool(p.R))

	for _, arg := range p.Args {
		parameters.Add("arg", arg)
	}

	return parameters.Encode()
}

func main() {
	cmd := LsCmd{
		Method: "ls",
		R:      false,
		Args:   []string{"/pfs/test1/", "/pfs/test2/"},
	}

	fmt.Println(cmd.ToUrlParam())
}
