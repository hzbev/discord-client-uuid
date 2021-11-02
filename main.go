package main

import (
	"encoding/base64"
	"encoding/binary"
	"fmt"
	"math/rand"
	"strconv"
	"time"
)

func main() {
	var randomPrefix int
	rand.Seed(time.Now().UnixNano())
	num := int(4294967296 * rand.Float64())
	creationTime := time.Now().Unix() * 1000
	if num <= 2147483647 {
		randomPrefix = num
	} else {
		randomPrefix = num - 4294967296
	}

	userID, _ := strconv.Atoi("USER ID HERE")
	firstTemp := func() int {
		if userID%4294967296 <= 2147483647 {
			return userID % 4294967296
		} else {
			return userID%4294967296 - 2147483647
		}
	}()
	first := make([]byte, 4)
	binary.LittleEndian.PutUint32(first, uint32(firstTemp))
	second := make([]byte, 4)
	binary.LittleEndian.PutUint32(second, uint32(userID>>32))
	third := make([]byte, 4)
	binary.LittleEndian.PutUint32(third, uint32(randomPrefix))
	fourthTemp := func() int64 {
		if creationTime%4294967296 <= 2147483647 {
			return creationTime % 4294967296
		} else {
			return creationTime%4294967296 - 2147483647
		}
	}()
	fourth := make([]byte, 4)
	binary.LittleEndian.PutUint32(fourth, uint32(fourthTemp))
	fifth := make([]byte, 4)
	binary.LittleEndian.PutUint32(fifth, uint32(creationTime>>32))
	sixth := make([]byte, 4)
	binary.LittleEndian.PutUint32(sixth, uint32(0))
	buf := make([]byte, 24)
	copy(buf[:], first)
	copy(buf[4:], second)
	copy(buf[8:], third)
	copy(buf[12:], fourth)
	copy(buf[16:], fifth)
	copy(buf[20:], sixth)

	fmt.Println(base64.StdEncoding.EncodeToString(buf))
}
