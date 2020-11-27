resource "azure_storage_container" "storage_container" {
  name                  = var.name
  container_access_type = var.containerAccessType
  storage_service_name  = var.storageServiceName
}
