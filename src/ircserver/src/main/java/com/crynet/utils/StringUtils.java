package com.crynet.utils;

public class StringUtils {
    public static String joinStringArray(String[] arr, int startIdx) {
        StringBuilder result = new StringBuilder();
        for (int i = startIdx; i < arr.length; i++) {
            if (i < arr.length - 1) {
                result.append(arr[i] + " ");
            } else {
                result.append(arr[i]);
            }
        }
        return result.toString();
    }
}
