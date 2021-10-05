import { Message } from '@/services/MqttClient'
import { computed, reactive } from '@vue/runtime-core'

const maxSize = 5
const messageStore = reactive<Array<Message>>([])

export const messages = computed(() => {
  return messageStore.slice().reverse()
})

export const addMessage = (message: Message) => {
  messageStore.push(message)
  if (messageStore.length > maxSize) {
    messageStore.shift()
  }
}

export const clearMessage = () => {
  messageStore.splice(0, messageStore.length)
}
