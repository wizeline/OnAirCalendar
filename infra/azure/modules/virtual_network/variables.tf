variable "name" {
  type = string
  description = "Virtual network name"
}

variable "resourceGroupName" {
  type = string
  description = "Resource group name"
}

variable "location" {
  type = string
  description = "Location"
}

variable "addressSpace" {
  type        = list(string)
  description = "List of address spaces"
}

variable "environment" {
  type = string
  description = "Environment value to be used on tags"
}
